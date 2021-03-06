Rational
В C++ существует множество встроенных типов данных для работы с целочисленной арифметикой и арифметикой над числами с плавающей точкой. Однако при вычислениях над последними возможны ошибки округления, которые могут накапливаться при сложных (состоящих из множества операций) вычислениях. Если при работе с иррациональными числами округлений избежать не получится, то в программах, требующих манипуляций только с рациональными числами, все вычисления можно свести к точной целочисленной арифметике.

Детали реализации
Рациональное число представляется двумя взаимно простыми целыми числами - числителем и положительным знаменателем. Публичный интерфейс класса включает:

Конструктор по умолчанию, который инициализирует число нулем
Неявный конструктор преобразования от целых чисел.
Конструктор от двух целых чисел (числитель и знаменатель).
Методы GetNumerator() и GetDenominator(), возвращающие числитель и знаменатель соответственно
Методы SetNumerator(int) и SetDenominator(int), устанавливающие числитель и знаменатель в требуемые значения
Бинарные арифметические операции (+, -, /, *) и их присваивающие версии
Унарные операции (+, -), а также префиксные и постфиксные инкременты и декременты
Операции сравнения
Операции ввода из потока и вывода в поток (>>, <<)
