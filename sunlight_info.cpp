#include "stdafx.h"
#include "sunlight_info.h"


const char* SunlightInfo::getText() const {
  return getText(state);
}

const char* SunlightInfo::getText(SunlightState state) {
  switch (state) {
    case SunlightState::NIGHT: return "night";
    case SunlightState::DAY: return "day";
  }
}

const auto dayLength = 1500_visible;
const auto nightLength = 1500_visible;
const auto duskLength  = 180_visible;

void SunlightInfo::update(GlobalTime currentTime) {
  GlobalTime d;
  while (1) {
    d += dayLength;
    if (d > currentTime) {
      lightAmount = 1;
      timeRemaining = d - currentTime;
      state = SunlightState::DAY;
      break;
    }
    d += duskLength;
    if (d > currentTime) {
      lightAmount = (d - currentTime).getDouble() / duskLength.getDouble();
      timeRemaining = d + nightLength - duskLength - currentTime;
      state = SunlightState::NIGHT;
      break;
    }
    d += nightLength - duskLength * 2;
    if (d > currentTime) {
      lightAmount = 0;
      timeRemaining = d + duskLength - currentTime;
      state = SunlightState::NIGHT;
      break;
    }
    d += duskLength;
    if (d > currentTime) {
      lightAmount = 1 - (d - currentTime).getDouble() / duskLength.getDouble();
      timeRemaining = d - currentTime;
      state = SunlightState::NIGHT;
      break;
    }
  }
}

SunlightState SunlightInfo::getState() const {
  return state;
}

double SunlightInfo::getLightAmount() const {
  return lightAmount;
}

TimeInterval SunlightInfo::getTimeRemaining() const {
  return timeRemaining;
}

