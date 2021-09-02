#ifndef TOOLS_H
#define TOOLS_H
#include <random>
#include <iterator>

namespace Tools
{
    static std::default_random_engine rng_engine = std::default_random_engine{};

    template<typename Iter, typename RandomGenerator>
    Iter GetRandom(Iter start, Iter end, RandomGenerator& gen)
    {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(gen));
        return start;
    }

    template<typename Iter>
    Iter GetRandom(Iter start, Iter end)
    {
        return GetRandom(start, end, rng_engine);
    }

    int GetRandomInt(int rangeStart = 0, int rangeEnd = 100)
    {
        std::uniform_int_distribution<> distr(rangeStart, rangeEnd);
        return distr(rng_engine);
    }

    // TODO NOW add https://github.com/Rain92/qt5-kobo-platform-plugin to the target and src
    // KoboDeviceDescriptor koboDevice = KoboPlatformFunctions::getKoboDeviceDescriptor();
}

#endif // TOOLS_H
