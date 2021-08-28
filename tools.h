#ifndef TOOLS_H
#define TOOLS_H
#include  <random>
#include  <iterator>

namespace Tools
{
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
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return GetRandom(start, end, gen);
    }

    int GetRandomInt(int rangeStart = 0, int rangeEnd = 100)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(rangeStart, rangeEnd);

        return distr(gen);
    }
}

#endif // TOOLS_H
