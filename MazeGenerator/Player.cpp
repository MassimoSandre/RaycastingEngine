#include "Player.h"
#include "utils/utils.cpp"
#include "entity/entities/MovingEntity.h"
#include <cmath>

Player::Player(double fov, int nRays, double raysLength)  {
	this->fov = fov;
	this->nRays = nRays;
	this->raysLength = raysLength;
	this->focalLength = ((1.0f / tan(fov / 2.0f)) / 2.0f);

	this->info.resize(nRays);
	for (int i = 0; i < nRays; i++) {
		this->info[i].resize(0);
	}
}

void Player::assignEntityState(EntityState state) {
	this->state = state;

	for (int i = 0; i < nRays; i++) {
		Ray r(state.position, raysLength, state.angle);
		rays.push_back(std::make_shared<Segment>(r));
	}
	this->pointRaysToView();
}


void Player::pointRaysToView() {
	double x = 0;
	double angle;

	for (int i = 0; i < this->nRays; i++) {
		x = (double(i) / double(this->nRays)) - 0.5;
		angle = atan2(x, this->focalLength);
		this->rays[i]->setAngle(this->state.angle + angle);
	}
}
void Player::castWall(ObstacleState& wall, int wallFace) {
	for (int i = 0; i < this->nRays; i++) {
		IntersectionInfo intersection;
		this->rays[i]->getIntersection(&intersection, &wall.segments[wallFace]);

		if (intersection.intersection.x < 0 || intersection.intersection.y < 0) continue;

		if (intersection.intersection.x == 0 && intersection.intersection.y == 0) continue;

		double d1 = intersection.intersection.distance(this->state.position);
		if (d1 == 0) break;

		if (d1 > this->rays[i]->length) continue;

		double d2 = intersection.intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;
		
		this->info[this->nRays - 1 - i].push_back({  d1 * (double)cos(this->rays[i]->angle - this->state.angle), intersection.colOffset, wall.height, wall.verticalOffset, wall.owner });

		this->rays[i]->changeP2(intersection.intersection);
	}
}

void Player::castEntity(EntityState& entity) {
	/*for (int i = 0; i < this->nRays; i++) {
		IntersectionInfo intersection = this->rays[i]->getIntersection(entity.segment);

		if (intersection.intersection.x < 0 || intersection.intersection.y < 0) continue;

		if (intersection.intersection.x == 0 && intersection.intersection.y == 0) continue;

		double d1 = intersection.intersection.distance(this->state.position);
		if (d1 == 0) break;

		if (d1 > this->rays[i]->length) continue;

		double d2 = intersection.intersection.distance(this->rays[i]->p2);
		if (d2 > this->rays[i]->length) continue;

		this->info[this->nRays - 1 - i].push_back({ d1 * (double)cos(this->rays[i]->angle - this->state.angle), intersection.colOffset, entity.height, entity.verticalOffset, entity.owner });
	}*/
	
}

void Player::pointTo(Coordinates p) {
	this->state.angle = this->state.position.getAngle(p);
	this->pointRaysToView();
}

void Player::rotate(double angle) {
	this->state.angle += angle;
		
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->setAngle(this->rays[i]->angle + angle);
	}
}

void Player::update() {
	if (this->nRays > 0) {
		if (this->state.position.x == this->rays[0]->p1.x && this->state.position.y == this->rays[0]->p1.y) return;
	}
	for (int i = 0; i < this->nRays; i++) {
		this->rays[i]->move(this->state.position);
	}
}
Segment Player::moveForward(double distance) {
	double angle = this->state.angle;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}
Segment Player::moveBackward(double distance) {
	double angle = this->state.angle + 3.1415;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}
Segment Player::moveLeftward(double distance) {
	double angle = this->state.angle + 3.1415 / 2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}
Segment Player::moveRightward(double distance) {
	double angle = this->state.angle - 3.1415 / 2;
	Coordinates o = { distance * cos(angle), -distance * sin(angle) };
	return MovingEntity::move(this->state, o);
}

#include <chrono>

void Player::cast(std::vector<ObstacleState>& states, std::vector<EntityState>& entities, double* a) {
	//IntersectionInfo info{0};
	////double distance;
	//
	////std::vector<int> a(rays.size());
	//for (int i = 0; i < rays.size(); i++) {
	//	rays[i]->setLength(raysLength);
	//	//a[i] = 0;
	//}

	//auto now1 = std::chrono::steady_clock::now();

	//
	//
	//for (ObstacleState& e : states) {	
	//	for (int i = 0; i < e.segments.size(); i++) {
	//		if (e.segments[i].length <= raysLength &&
	//			e.segments[i].p1.distance(rays[0]->p1) >= 1.5 * raysLength &&
	//			e.segments[i].p2.distance(rays[0]->p1) >= 1.5 * raysLength) continue;
	//		
	//		//castWall(e, i);
	//		for (int j = 0; j < rays.size(); j++)  {
	//			
	//			//rays[j]->getIntersection(&info,&e.segments[i]);
	//			
	//			double a1, b1, c1, a2, b2, c2, den;

	//			a1 = (rays[j]->p2.y) - (rays[j]->p1.y);
	//			b1 = (rays[j]->p1.x) - (rays[j]->p2.x);
	//			c1 = a1 * (rays[j]->p1.x) + b1 * (rays[j]->p1.y);

	//			a2 = (e.segments[i].p2.y) - (e.segments[i].p1.y);
	//			b2 = (e.segments[i].p1.x) - (e.segments[i].p2.x);
	//			c2 = a2 * (e.segments[i].p1.x) + b2 * (e.segments[i].p1.y);

	//			den = a1 * b2 - a2 * b1;


	//			if (den == 0.0) {
	//				info.intersection = { -1,-1 };
	//			}
	//			else {

	//				info.intersection = { ((b2 * c1 - b1 * c2) / den), ((a1 * c2 - a2 * c1) / den) };


	//				info.colOffset = e.segments[i].p1.distance(info.intersection);


	//				if (info.colOffset > e.segments[i].length) {
	//					info.intersection = { -1,-1 };
	//				}
	//				else {
	//					if (e.segments[i].p2.distance(info.intersection) > e.segments[i].length) {
	//						info.intersection = { -1,-1 };
	//					}
	//					else {
	//						info.colOffset -= e.segments[i].length / 2.0;
	//					}
	//				}
	//			}

	//			if (info.intersection.x < 0 || info.intersection.y < 0) continue;
	//			
	//			if (info.intersection.x == 0 && info.intersection.y == 0) continue;

	//			double d1 = info.intersection.distance(rays[0]->p1);
	//			if (d1 == 0) continue;

	//			if (d1 > rays[j]->length) continue;
	//			
	//			double d2 = info.intersection.distance(rays[j]->p2);
	//			if (d2 > rays[j]->length) continue;

	//			//if (info[rays.size() - 1 - j].size() >= 1) {
	//			//	//info[nRays - 1 - j][0] = RenderInfo{d1 * (double)cos(rays[j]->angle - state.angle), info.colOffset, e.height, e.verticalOffset, e.owner};
	//			//	//info[nRays - 1 - j][0].distance =0;//.distance = d1 * (double)cos(rays[j]->angle - state.angle);
	//			//	//fov = 90;
	//			//	
	//			//}
	//			//else {
	//			//	//info[nRays - 1 - j].push_back({ d1 * (double)cos(rays[j]->angle - state.angle), info.colOffset, e.height, e.verticalOffset, e.owner });
	//			//}
	//			//a[j] = RenderInfo{d1 /* (double)cos(rays[j]->angle - state.angle)*/, info.colOffset, e.height, e.verticalOffset, e.owner};
	//			//rays[j]->p1.x = 1;
	//			//rays[j]->changeP2(info.intersection); 
	//			//rays[j]->calculateP2();
	//			//distance = 0;
	//			a[j] = d1 * (double)cos(rays[j]->angle - state.angle);
	//		}
	//		
	//	}
	//	
	//}

	////for (EntityState& e : entities) {
	////	if (e.segment.length <= raysLength &&
	////		e.segment.p1.distance(state.position) >= 2 * raysLength &&
	////		e.segment.p2.distance(state.position) >= 2 * raysLength) continue;

	////		//castEntity(e);
	////}


	//auto now2 = std::chrono::steady_clock::now();
	//double updateTime = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now2 - now1).count();
	//std::cout << updateTime << std::endl;
}

ViewInfo Player::getFixedDistances() {
	return this->info;
}