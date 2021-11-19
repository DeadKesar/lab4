//Разработать программу - калькулятор, которая способна сохранять любое количество именованных пользовательских переменных и использовать их для расчета простейших алгебраических выражений.Предусмотреть возможность определения любой из переменных простейшим алгебраическим выражением.Например:
//exrp = first - second + third
//someRes = expr + 10
//first = 10
//Предусмотреть возможность вывода любой переменной на экран.Если есть выражения, переменные которых не определены, то пользователю выводится это выражение и предлагается ввести все необходимые значения переменных с консоли.

#include <iostream>
struct SomeStruct
{
    char* variableName;
    int nameLength;
    int numberOfUsing = 0; //количество использований
    double value;


};
// связный список, операноры - связь переменных.
struct ValueLink
{
    SomeStruct value;
    OperatorLink & operBefore;
    OperatorLink & operAfter;
    //OperatorLink tail; 
};

struct OperatorLink
{
    ValueLink & valueBefore;
    ValueLink & valueAfter;
    char oper[];
};

class Expression
{
public:
    ValueLink value;

};

class MyStringBuilder
{
    int sizeOfMemory = 8, currentSize = 1;
    char* string = new char[sizeOfMemory]();
public:
    MyStringBuilder()
    {
        int sizeOfMemory = 8, currentSize = 1;
        char* string = new char [sizeOfMemory]();
    }
    ~MyStringBuilder()
    {
        delete[] string;
    }
    void Add(char ch)
    {
        if (this->currentSize == this->sizeOfMemory)
        {
            string = copy(this->string, this->currentSize * 2, this->currentSize);
        }

        this->string[this->currentSize-1] = ch;
        this->string[this->currentSize] = '\0';
        this->currentSize++;
    }
    int Length()
    {
        return currentSize-1;
    }
    char operator [] (const int index)
    {
        return string[index];
    }
    char* Get()
    {
        return this->string;
    }

private:
    char* copy(char* str, int newSize, int curentSize)
    {
        char* tempStr = new char [newSize]();
        for (int i = 0; i < curentSize; i++)
        {
            tempStr[i] = str[i];
        }
        delete[] str;
        this->sizeOfMemory = newSize;
        str = tempStr;
        return str;
    }
};

class MyList
{
    int sizeOfMemory = 1, currentSize = 0;
    char** list = new char* [sizeOfMemory];

public:
    MyList()
    {
        int sizeOfMemory = 1, currentSize = 0;
        char** list = new char* [sizeOfMemory];
    }
    ~MyList()
    {
        for (int i = 0; i < this->Length(); i++)
        {
            delete[] list[i];
        }
        delete[] list;
    }
    void Add(char* str, int length)
    {
        if (this->currentSize == this->sizeOfMemory)
        {
            list = AddMoreMemory(this->list, this->currentSize * 2, this->currentSize);
        }
        char* tempStr = new char[length+1];
        for (int i = 0; i < length; i++)
        {
            tempStr[i] = str[i];
        }
        tempStr[length] = '\0';
        this->list[this->currentSize] = tempStr;
        this->currentSize++;
    }
    int Length()
    {
        return currentSize;
    }
    char* operator [] (const int index)
    {
        return list[index];
    }

private:
    char** AddMoreMemory(char** list, int newSize, int curentSize)
    {
        char** tempVector = new char* [newSize];
        for (int i = 0; i < curentSize; i++)
        {
            tempVector[i] = list[i];
        }
        delete[] list;
        this->sizeOfMemory = newSize;
        list = tempVector;
        return list;
    }
};

void GetInput(MyList & vector);
bool IsNumber(const char str[], int length);



int main()
{
    char* test1 = new char [] {"exrp = first - second + third + 10"};

    MyList arr;
    GetInput(arr);
    



    for (int i = 0; i < arr.Length(); i++)
    {
        for( int j = 0;*(arr[i] + j) != '\0';j++)
        {
            std::cout << *(arr[i] + j);
        }
        std::cout << std::endl;
    }

}

Expression MakeExpression(char str[], int length)
{
    bool isWord = false, isFirst = true;
    int index = 0, startWord = 0, wordLength = 0;
    while (isFirst && index < length)
    {
         if (!(str[index] == ' '))
         {
             startWord = index;
             isWord = true;
         }
         else if (isWord)
         {
             wordLength = index;
         }

         index++;
    }

    for (int i = 0; i < length; i++)
    {
        if (str[i])

    }
}

bool IsNumber(const char str[], int length)
{
    if (length > 0)
    {
        for (int i = 0; i < length; i++)
        {
            if (!(str[i] >= '0' && str[i] <= '9'))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}


void GetInput(MyList & list)
{
    
    bool isNotEnd = true;
    while (isNotEnd)
    {
        MyStringBuilder str;
        char ch = ' ';
        while (ch != '\n')
        {
            ch = getchar();
            if (ch == '\n')
                continue;
            if (ch == (char)26 || ch == (char)-1)
            {
                isNotEnd = false;
                break;
            }
            str.Add(ch);
        }
        if (str.Length() > 0)
        {
            list.Add(str.Get(),str.Length());
        }
    }
}