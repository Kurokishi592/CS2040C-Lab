#include "food.h"
#include <iostream>
using namespace std;

Food Food:: operator+(const Food& rhs) const
{
    return Food(m_name + " " + rhs.m_name, m_calories+rhs.m_calories);
}


bool Food:: operator<(const Food& rhs) const
{
    return m_calories < rhs.m_calories;
}

bool Food:: operator==(const Food& rhs) const
{
    return m_name == rhs.m_name && m_calories == rhs.m_calories;
}
