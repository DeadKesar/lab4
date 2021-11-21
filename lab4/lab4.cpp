//Разработать программу - калькулятор, которая способна сохранять любое количество именованных пользовательских переменных и использовать их для расчета простейших алгебраических выражений.Предусмотреть возможность определения любой из переменных простейшим алгебраическим выражением.Например:
//exrp = first - second + third
//someRes = expr + 10
//first = 10
//Предусмотреть возможность вывода любой переменной на экран.Если есть выражения, переменные которых не определены, то пользователю выводится это выражение и предлагается ввести все необходимые значения переменных с консоли.
 

#include <iostream>
#include <sstream>



struct SomeStruct;
struct ValueNode;
struct OperatorNode;
class Expression;
class MyList;
class MyStringBuilder;

void GetInput(MyList& vector);
bool IsNumber(const char str[], int length);
char* GetSubstring(char text[], int start, int size);
OperatorNode FindOperator(char str[], int& index, int length);
double GetNumFromString(const char str[]);

struct SomeStruct
{
    bool hasValue = false;
    char* variableName;
    int nameLength;
    int numberOfUsing = 0; //количество использований
    double value;
    SomeStruct()
    {   }
    SomeStruct(char name[], int nameLength, double value)
    {
        this->variableName = name;
        this->nameLength = nameLength;
        this->value = value;
    }
    ~SomeStruct()
    {
        delete[] variableName;
    }
    void Set(char name[], int nameLength)
    {
        this->variableName = name;
        this->nameLength = nameLength;
    }
    void Set(char name[], int nameLength, double value)
    {
        this->variableName = name;
        this->nameLength = nameLength;
        this->value = value;
        this->hasValue = true;
    }

    void findValue(char str[], int& index, int length)
    {
        int startWord = 0, wordLength = 0;
        bool isWord = false;
        while (index < length)
        {
            if (!(isWord || str[index] == ' '))
            {
                startWord = index;
                isWord = true;
            }
            if (isWord && (str[index] == ' ' || str[index] == '+' || str[index] == '-' || str[index] == '*' || str[index] == '/' || str[index] == '=' || str[index] == '%'))
            {
                wordLength = index - startWord;
                char* tempStr = GetSubstring(str, startWord, wordLength);
                if (IsNumber(tempStr, wordLength))
                {
                    this->Set(tempStr, wordLength, GetNumFromString(tempStr));
                }
                else
                {
                    this->Set(tempStr, wordLength);
                }
                break;
            }
            index++;
        }
    }
};
// связный список, операноры - связь переменных.
struct ValueNode
{
    bool isHead = false;
    bool isTail = false;
    SomeStruct *value;
    OperatorNode *operBefore;
    OperatorNode *operAfter;
    //OperatorNode tail; 
};

struct OperatorNode
{
    ValueNode * valueBefore;
    ValueNode * valueAfter;
    char* oper;
    OperatorNode()
    {  };
};

class Expression
{
    int length = 0;
public:
    ValueNode * valueHead;
    ValueNode * valueTail;
    void Add(SomeStruct &val, OperatorNode &oper)
    {
        ValueNode temp;
        temp.value = &val;
        if (length == 0)
        {
            this->valueHead = &temp;
            valueHead->isHead = true;
            valueHead->operAfter = &oper;
            oper.valueBefore = valueHead;
            this->valueHead->isTail = true;
            valueTail = valueHead;
            length++;
        }
        else
        {
            temp.operBefore = valueTail->operAfter;
            this->valueTail->isTail = false;
            this->valueTail->operAfter->valueAfter = &temp;
            temp.operAfter = &oper;
            oper.valueBefore = &temp;
            valueTail = &temp;
            this->valueTail->isTail = true;
            length++;
        }
    }
    void MakeExpression(char str[], int length)
    {
        SomeStruct variable;
        OperatorNode oper;
        for (int i = 0; i < length; i++)
        {
            variable.findValue(str, i, length);
            oper = FindOperator(str, i, length);
            this->Add(variable, oper);
        }
    }
private:


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
    int ElementLength(const int index)
    {
        int length = 0;
        for (; *(list[index] + length) != '\0'; length++)
        {    }
        return length;
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

int main()
{
    char* test1 = new char [] {"exrp"};


    MyList arr;
    GetInput(arr);
    Expression ex;
    ex.MakeExpression(arr[0], arr.ElementLength(0));



    for (int i = 0; i < arr.Length(); i++)
    {
        for( int j = 0;*(arr[i] + j) != '\0';j++)
        {
            std::cout << *(arr[i] + j);
        }
        std::cout << std::endl;
    }

}

  

OperatorNode FindOperator(char str[], int& index, int length)
{
    OperatorNode oper;
    while (true)
    {
        if (str[index] == '+' || str[index] == '-' || str[index] == '*' || str[index] == '/' || str[index] == '=' || str[index] == '%')
        {
            if (str[index + 1] == '=')
            {
                oper.oper = GetSubstring(str, index, 2);
                index += 2;
                return oper;
            }
            oper.oper = GetSubstring(str, index, 1);
            index++;
            return oper;
        }
        else if (str[index == ' '])
        {
            index++;
            continue;
        }
    }
}

char* GetSubstring(char text[], int start, int size)
{
    char* str = new char[size + 1];
    for (int i = 0; i < size; i++)
    {
        str[i] = text[i + start];
    }
    str[size] = '\0';
    return str;
}

bool IsNumber(const char str[], int length)
{
    bool isFloat = false;
    if (length > 0)
    {
        for (int i = 0; i < length; i++)
        {
            if (!(str[i] >= '0' && str[i] <= '9' || str[i] == '.' || str[i] == ','))
            {
                return false;
            }
            if (str[i] == '.' || str[i] == ',')
            {
                if (isFloat)
                {
                    return false;
                }
                isFloat = true;
            }
        }
        return true;
    }
    return false;
}
double GetNumFromString(const char str[])
{
    double answ = 0;
    std::istringstream iss(str);
    iss >> answ;
    return answ;
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