#include <string>
#include <utility>
#include "Match.h"
#include "Player.h"
#include <common/Sizes.h>
#include <model/DTO_Info.h>
#include <common/SocketError.h>
#include "ThreadClientEventMonitor.h"
#include <server/FramesSyncronizer.h>

#define MAP_PATH "maps/"
#define MAP_NAME "track_01.json"
#define PLUGINS_PATH "plugings/"

#define ERROR_MATCH_JOIN_FLAG 1

#define OPEN_MATCH_FLAG "0"
#define CLOSE_MATCH_FLAG "1"

#define SUCCESS_MATCH_JOIN_FLAG 0

#define START_MATCH_FLAG 0
#define TIME_START 3

#define RESTART_RACE_OPTION 1

#define UPDATE_QUEUE_MAX_SIZE 1000

Match::Match(std::string match_creator, std::string match_name):
        closed(false),
        updates_race(10000),
        match_name(std::move(match_name)),
        race(3, MAP_PATH, MAP_NAME),
        match_creator(std::move(match_creator)),
        clients_monitor(this, this->updates_race),
        plugins_manager(this->race, PLUGINS_PATH),
        waiting_restart(false),
        timer(TIME_START,this->race, this->client_updater)
{}

void Match::add_player(Player&& player) {
    try {
        if (this->running)
            player.send((uint8_t)ERROR_MATCH_JOIN_FLAG);
        else {
            player.send((uint8_t)SUCCESS_MATCH_JOIN_FLAG);
            player.set_ID((int32_t)players.size());
            this->players.emplace(players.size(),std::move(player));
        }
    } catch (const SocketError& exception) {
        std::cerr << "Player disconnected during match creation\n";
    }
}

std::string Match::get_match_name_to_send() {
    std::string match_name_to_send;
    match_name_to_send.append(this->match_name + " ");
    match_name_to_send.append("Created by: " + this->match_creator);
    match_name_to_send.append(this->running ? CLOSE_MATCH_FLAG : OPEN_MATCH_FLAG);
    match_name_to_send.append("\n");
    return match_name_to_send;
}

bool Match::has_username(std::string& username) {
    for (auto& player : this->players) {
        if (player.second.is_called(username)) {
            return true;
        }
    }
    /* Match creator is added at the end*/
    return this->match_creator == username;
}

void Match::kill() {
    for (auto &player : this->players) {
        player.second.kill();
    }
    this->remove_disconnected_players();
}

void Match::close() {
    /*Close is used by the MatchTable,
    a non started  match is also not running,
    but not dead*/
    this->timer.join();
    this->updates_race.close();
    this->clients_monitor.join();
    this->running = false;
    this->closed = true;

    std::cout << "CERRE TODO\n";
}

bool Match::is_closed() {
    return this->closed;
}

void Match::initialize_players() {
    for (auto& player : players) {
        int32_t ID = player.first;

        CarSpecs specs{100, 120, -20, 130, 150, 8.5, 7.5};
        this->race.add_car_with_specs(player.first, specs);

        this->client_updater.create_update_queue(ID, UPDATE_QUEUE_MAX_SIZE);
        this->thread_players.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
                                                               std::forward_as_tuple(this->client_updater,
                                                                                      updates_race, player.second));

        try {
            this->players.at(ID).send((uint8_t)START_MATCH_FLAG);
            this->thread_players.at(ID).start();
            this->race.send_info_to_player(ID, this->client_updater);
        } catch (const SocketError& exception) {
            std::cerr << "Player disconnected\n";
            this->players.erase(player.first);
            this->thread_players.at(ID).join();
            this->thread_players.erase(ID);
            this->race.player_left_game(ID);
        }
    }
    this->race.prepare(this->client_updater);
}

void Match::remove_disconnected_players() {
    bool creator_disconnected = false;
    std::unique_lock<std::mutex> lock(mtx);
    for (auto th_player = this->thread_players.begin(); th_player != this->thread_players.end();) {
        int32_t ID = (*th_player).first;
        if ((*th_player).second.dead()) {
            (*th_player).second.join();
            this->race.player_left_game(ID);
            if (this->players.at(ID).is_called(this->match_creator))
                creator_disconnected = true;
            this->players.erase(ID);
            this->client_updater.remove_queue(ID);
            th_player =  this->thread_players.erase(th_player);
        } else {
            th_player++;
        }
    }
    if (creator_disconnected)
        this->select_new_creator();
}

void Match::apply_update(UpdateRace update) {
    std::unique_lock<std::mutex> lock(mtx);
    this->race.update_cars(update);
    update.update_player_view(this->players);
}

void Match::step() {
    FramesSynchronizer::sync_FPS(FRAMES_PER_SECOND);
    std::unique_lock<std::mutex> lock(mtx);
    //this->plugins_manager.execute();
    this->race.update();
}

void Match::update_players() {
    for (auto& player : this->players) {
        int32_t ID = player.first;

        this->race.send_updates(ID, this->client_updater);

        if (this->players.at(ID).is_playing() && this->race.car_complete_laps(ID))
             this->players.at(ID).set_finished(this->client_updater);

        if (!this->players.at(ID).is_playing())
            this->players.at(ID).update_view(players.size(), client_updater);
    }
}

void Match::wait_match_creator_decision() {
    int32_t creator_ID = 0;

    for (auto& player : this->players) {
        if (player.second.is_called(this->match_creator)) {
            creator_ID = player.first;
        }
    }

    for (auto& th_player : this->thread_players) {
        if (th_player.first != creator_ID) {
            th_player.second.set_on_hold();
        }
    }

    this->clients_monitor.set_on_restart_mode();
    //Aca mando un msj
    this->waiting_restart = true;
}

bool Match::all_players_finished() {
    for (auto& player : this->players) {
        if (player.second.is_playing())
            return false;
    }
    return true;
}

void Match::run() {
    this->plugins_manager.load_plugings();
    this->initialize_players();

    this->clients_monitor.start();
    this->timer.start();

    while (this->running) {
        this->step();
        this->update_players();
        this->remove_disconnected_players();

        if (this->players.empty()) {
            this->close();
            return;
        }

        if (!waiting_restart && this->all_players_finished())
            this->wait_match_creator_decision();
    }
}

void Match::set_restart_option(UpdateRace update) {
    int32_t creator_ID = 0;

    std::unique_lock<std::mutex> lock(mtx);
    for (auto& player : this->players) {
        if (player.second.is_called(this->match_creator))
            creator_ID = player.first;
    }

    if (!update.is_from(creator_ID))
        return;

    this->clients_monitor.set_on_running_game_mode();

    if (update.apply_restart_option(this->race))
        this->restart_match();
    else {
        /*When all players are killed,
        all threads close RAII */
        for (auto& player : this->players)
            player.second.kill();
    }
}

void Match::restart_match() {
    this->timer.join();

    for (auto& th_player : this->thread_players)
        th_player.second.resume();

    for (auto& player : this->players)
        player.second.restart_playing(this->client_updater);

    this->waiting_restart = false;
    this->timer.start();
}

void Match::select_new_creator() {
    //The new match creator is the first of the list
    if (!this->players.empty())
        this->match_creator = (*this->players.begin()).second.get_username();
}

