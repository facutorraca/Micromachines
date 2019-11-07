#include <vector>
#include <random>
#include "Podium.h"
#include "RacingTrack.h"
#include <common/MsgTypes.h>
#include <server/MapLoader.h>
#include "StaticTrackObject.h"
#include <common/EntityType.h>
#include <common/ProtectedQueue.h>

int get_random_position(int max_position) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, max_position);
    return dist(mt);
}


RacingTrack::RacingTrack(std::string& map_path, std::string& map_name):
    racing_track(b2Vec2(0,0))
{
    this->width = 0; //Default
    this->height = 0; //Default
    this->podium = nullptr;
    this->finish_line = nullptr;

    /*--------HYPERPARAMS----------*/
    this->velocity_iterations = 6;
    this->position_iterations = 2;
    this->time_step = 1.0f / 60.0f;

    this->track_terrain = TYPE_GRASS; //Default
    this->racing_track.SetContactListener(&this->contact_listener);

    MapLoader map_loader(map_path);
    map_loader.load_map(*this, "track_01.json");
}

void RacingTrack::update() {
    this->racing_track.Step(this->time_step,
                      this->velocity_iterations,
                      this->position_iterations);
}

void RacingTrack::send(ClientUpdater& client_updater, int32_t ID) {
    UpdateClient update_map_info({MSG_SET_BACKGROUND ,this->track_terrain, this->height, this->width});
    client_updater.send_to(ID, update_map_info);

    for (auto& track_part : this->track) {
        UpdateClient update_map = track_part->get_to_send();
        client_updater.send_to(ID, update_map);
    }
    for (auto& terrain : this->terrains) {
        UpdateClient update_map = terrain->get_to_send();
        client_updater.send_to(ID, update_map);
    }
    for (auto& object : this->static_track_objects) {
        UpdateClient update_map = object.get_to_send();
        client_updater.send_to(ID, update_map);
    }
}

void RacingTrack::add_terrain(std::unique_ptr<Terrain>&& terrain) {
    this->terrains.push_back(std::move(terrain));
    this->terrains.back()->add_to_world(this->racing_track);
}

void RacingTrack::add_track(std::unique_ptr<Terrain>&& track_part) {
    this->track.push_back(std::move(track_part));
    this->track.back()->add_to_world(this->racing_track);
}


void RacingTrack::add_static_track_object(StaticTrackObject&& object) {
    this->static_track_objects.push_back(std::move(object));
    this->static_track_objects.back().add_to_world(this->racing_track);
}

void RacingTrack::set_track_size(int32_t track_height, int32_t track_width) {
    this->width = track_width;
    this->height = track_height;
}

void RacingTrack::set_track_terrain(int32_t terrain) {
    this->track_terrain = terrain;
}

void RacingTrack::set_finish_line(Coordinate begin, Coordinate end) {
    this->finish_line = new FinishLine(begin, end, this->racing_track);
}

void RacingTrack::set_podium(Coordinate f_place, Coordinate s_place, Coordinate t_place) {
    this->podium = new Podium(f_place, s_place, t_place);
}

void RacingTrack::add_car(Car &car) {
    car.add_to_world(this->racing_track);
}

void RacingTrack::add_car_to_podium(Car &car, int32_t ID) {
    if (this->podium)
        this->podium->add_car(car, ID);
}

void RacingTrack::add_spawn_point(Coordinate spawn_point) {
    this->spawn_points.push_back(spawn_point);
}

void RacingTrack::set_spawn_points_to_cars(std::unordered_map<int32_t, Car> &cars) {
    int spawn_point_pos = (int)this->spawn_points.size() - 1; //Start from end to beginning
    for (auto &car : cars) {
        car.second.set_spawn_point(this->spawn_points[spawn_point_pos]);
        spawn_point_pos--;
    }
}

Coordinate RacingTrack::get_random_track_position() {
    int random_position = get_random_position(this->track.size()-1);
    return this->track.at(random_position)->get_map_coordinate();
}

RacingTrack::~RacingTrack() {
    delete this->podium;
    delete this->finish_line;
}

void RacingTrack::add_modifier(std::shared_ptr<Modifier> modifier) {
    modifier->add_to_world(this->racing_track);
}

