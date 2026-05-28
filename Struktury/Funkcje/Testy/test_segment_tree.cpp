#include "../../SegmentTree.h"
#include <iostream>

// Test 1: Przykład z wykładu (indeksowanie od 0)
bool test1()
{
    SegmentTree t1({5, 2, 7, 1, 3, 6, 4, 8});
    const int expected = 8;
    return (t1.query(2, 7) == expected);
}

// Test 2: Twój test dla innego przedziału
bool test2()
{
    SegmentTree t2({2, 3, -1, 5, -2, 4, 8, 10});
    const int expected = 5;
    return (t2.query(2, 4) == expected);
}

// Test 3: Same wartości ujemne (sprawdzenie czy poprawnie zwraca wartości poniżej 0)
bool test3()
{
    SegmentTree t3({-10, -5, -20, -3, -15});
    const int expected = -3;
    return (t3.query(0, 4) == expected);
}

// Test 4: Zapytanie o przedział jednoelementowy (L == R)
bool test4()
{
    SegmentTree t4({1, 5, 9, 2});
    const int expected = 9;
    return (t4.query(2, 2) == expected);
}

// Test 5: Test operacji UPDATE (mala wartosc na olbrzymia)
bool test5()
{
    SegmentTree t5({1, 2, 3, 4});
    t5.update(1, 100); 
    const int expected = 100;
    return (t5.query(0, 3) == expected);
}

// Test 6: Test operacji UPDATE, która zmniejsza maksimum
bool test6()
{
    SegmentTree t6({5, 20, 10});
    t6.update(1, 2); 
    const int expected = 10;
    return (t6.query(0, 2) == expected);
}

int main() {
    std::cout << "Testy dla drzewa przedzialowego (MaxQuery):" << std::endl;
    std::cout << "Test 1: " << (test1() ? "OK" : "ERROR") << " (Expected: 8)" << std::endl;
    std::cout << "Test 2: " << (test2() ? "OK" : "ERROR") << " (Expected: 5)" << std::endl;
    std::cout << "Test 3: " << (test3() ? "OK" : "ERROR") << " (Expected: -3)" << std::endl;
    std::cout << "Test 4: " << (test4() ? "OK" : "ERROR") << " (Expected: 9)" << std::endl;
    std::cout << "Test 5: " << (test5() ? "OK" : "ERROR") << " (Expected: 100 po update)" << std::endl;
    std::cout << "Test 6: " << (test6() ? "OK" : "ERROR") << " (Expected: 10 po zmniejszeniu max)" << std::endl;

    return 0;
}