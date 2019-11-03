#include <vector>
#include <common/MsgTypes.h>
#include <common/EntityType.h>
#include "StaticTrackObject.h"
#include <common/ProtectedQueue.h>
#include <server/MapLoader.h>
#include "RacingTrack.h"
#include "Podium.h"



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

void RacingTrack::send(ProtectedQueue<UpdateClient>& player_queue) {
    UpdateClient update_map_info({MSG_SET_BACKGROUND ,this->track_terrain, this->height, this->width});
    player_queue.push(update_map_info);

    for (auto& terrain : this->terrains) {
        UpdateClient update_map = terrain->get_to_send();
        player_queue.push(update_map);
    }
    for (auto& object : this->static_track_objects) {
        UpdateClient update_map = object.get_to_send();
        player_queue.push(update_map);
    }
}

void RacingTrack::add_terrain(std::unique_ptr<Terrain>&& terrain) {
    this->terrains.push_back(std::move(terrain));
    this->terrains.back()->add_to_world(this->racing_track);
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

void RacingTrack::add_car_to_podium(Car &car) {
    this->podium->add_car(car);
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

RacingTrack::~RacingTrack() {
    delete this->podium;
    delete this->finish_line;
}
