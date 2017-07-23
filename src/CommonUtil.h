#pragma once

#include "ofMain.h"

class SmoothValue {
public:
	SmoothValue() :mSpeed(0.02), mValue(0), mTarget(0) {}

	void update() {
		mValue += (mTarget - mValue) * mSpeed;
	}

	void to(float target) {
		mTarget = target;
	}

	void set(float target) {
		mValue = target;
		mTarget = target;
	}

	float get() {
		return mValue;
	};

	void setSpeed(float speed) {
		mSpeed = speed;
	}

private:
	float mValue;
	float mTarget;
	float mSpeed;
};

class SmoothPoint : public ofPoint {
public:
	SmoothPoint() :mSpeed(0.02) {}

	void update() {
		this->x += (mTarget.x - this->x) * mSpeed;
		this->y += (mTarget.y - this->y) * mSpeed;
		this->z += (mTarget.z - this->z) * mSpeed;
	}

	void to(ofPoint target) {
		mTarget = target;
	}
	void toRandom(ofPoint& min, ofPoint& max) {
		this->to(ofPoint(
			ofRandom(min.x, max.x), ofRandom(min.y, max.y), ofRandom(min.z, max.z)
		));
	}

	void set(ofPoint target) {
		mTarget = target;
		ofPoint::set(target);
	}

	void setSpeed(float speed) {
		mSpeed = speed;
	}

private:
	ofPoint mTarget;
	float mSpeed;
};
