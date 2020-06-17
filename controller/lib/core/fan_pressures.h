/* Copyright 2020, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef FAN_PRESSURES_H
#define FAN_PRESSURES_H

#include "units.h"
#include <algorithm>
#include <array>
#include <utility>

// Steady-state fan pressure recorded at various fan power levels.
//
// This data comes from sample-data/2020-06-15-fan-pressure, but with some
// cleanups.
//
//  - We average together the last 32 readings in each file.
//
//  - Power levels below 35% are removed.  The fan is barely spinning at those
//    power levels; it doesn't generate a meaningful amount of pressure.
//
//  - Power levels in range [94%, 100%) are removed.
//
//    Above 94% our measured pressure stops increasing.  The fan runs audibly
//    louder as you go from 86% to 100%, so my guess is that pressure *is*
//    increasing, but my test setup was unable to detect this due to an
//    imperfect seal on the expiratory limb.
//
//    Evidence for this is that I can feel a bit of air leaving through the
//    hose.  Evidence *against* this is that when I seal the hose with my
//    finger, I don't see any change in pressure!
//
inline constexpr std::array<std::pair<float, Pressure>, 49> FAN_PRESSURES{{
    {0.35f, cmH2O(1.92f)},  {0.40f, cmH2O(3.89f)},  {0.45f, cmH2O(6.09f)},
    {0.50f, cmH2O(8.68f)},  {0.51f, cmH2O(9.00f)},  {0.52f, cmH2O(9.34f)},
    {0.53f, cmH2O(10.06f)}, {0.54f, cmH2O(10.31f)}, {0.55f, cmH2O(11.02f)},
    {0.56f, cmH2O(11.71f)}, {0.57f, cmH2O(12.17f)}, {0.58f, cmH2O(12.36f)},
    {0.59f, cmH2O(13.22f)}, {0.60f, cmH2O(13.43f)}, {0.61f, cmH2O(14.37f)},
    {0.62f, cmH2O(14.71f)}, {0.63f, cmH2O(14.96f)}, {0.64f, cmH2O(15.73f)},
    {0.65f, cmH2O(16.33f)}, {0.66f, cmH2O(16.67f)}, {0.67f, cmH2O(17.92f)},
    {0.68f, cmH2O(18.34f)}, {0.69f, cmH2O(19.12f)}, {0.70f, cmH2O(19.72f)},
    {0.71f, cmH2O(20.24f)}, {0.72f, cmH2O(20.76f)}, {0.73f, cmH2O(21.55f)},
    {0.74f, cmH2O(22.28f)}, {0.75f, cmH2O(23.41f)}, {0.76f, cmH2O(23.89f)},
    {0.77f, cmH2O(24.39f)}, {0.78f, cmH2O(25.56f)}, {0.79f, cmH2O(27.15f)},
    {0.80f, cmH2O(27.89f)}, {0.81f, cmH2O(28.97f)}, {0.82f, cmH2O(30.94f)},
    {0.83f, cmH2O(33.00f)}, {0.84f, cmH2O(34.31f)}, {0.85f, cmH2O(35.87f)},
	{0.86f, cmH2O(37.5f)}, {0.87f, cmH2O(39.2f)}, {0.88f, cmH2O(41.1f)},
	{0.89f, cmH2O(42.9f)}, {0.90f, cmH2O(45.3f)}, {0.91f, cmH2O(47.0f)},
	{0.92f, cmH2O(49.1f)}, {0.93f, cmH2O(51.5f)}, {0.94f, cmH2O(53.5f)},
	{1.00f, cmH2O(54.8f)},
}};

// Check at compile time that the FAN_PRESSURES array is monotonic in both the
// fan speed (first element of the pair) and fan pressure (second element).
//
// This lets us binary search with confidence!
static_assert([] {
  for (int i = 1; i < FAN_PRESSURES.size(); i++) {
    auto p0 = FAN_PRESSURES[i - 1];
    auto p1 = FAN_PRESSURES[i];
    if (p0.first > p1.first || p0.second > p1.second) {
      return false;
    }
  }
  return true;
}());

// Finds the lowest fan power which achieves at least this pressure.
inline float FanPowerFor(Pressure p) {
  auto it = std::lower_bound(
      FAN_PRESSURES.begin(), FAN_PRESSURES.end(), p,
      [&](const auto &elem, const Pressure &p) { return elem.second < p; });

  if (it == FAN_PRESSURES.end()) {
    it = FAN_PRESSURES.end() - 1;
  }
  return it->first;
}

#endif // FAN_PRESSURES_H
