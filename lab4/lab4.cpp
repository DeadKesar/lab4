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
struct ExpressionArray;
class Expression;
class MyList;
class MyStringBuilder;

void GetInput(MyList& vector);
bool IsNumber(const char str[], int length);
char* GetSubstring(char text[], int start, int size);
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
                TakeValue(str, index, startWord);
                isWord = false;
                break;
            }
            index++;
        }
        if (isWord)
        {
            TakeValue(str, index, startWord);
        }
    }
    void TakeValue(char* str, int index, int startWord)
    {
        int wordLength = index - startWord;
        char* tempStr = GetSubstring(str, startWord, wordLength);
        if (IsNumber(tempStr, wordLength))
        {
            this->Set(tempStr, wordLength, GetNumFromString(tempStr));
        }
        else
        {
            this->Set(tempStr, wordLength);
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
};

struct OperatorNode
{
    ValueNode * valueBefore;
    ValueNode * valueAfter;
    char* oper;
    int length = 0;
    OperatorNode()
    {  };
    ~OperatorNode()
    {
        delete[] oper;
    }
    void FindOperator(char str[], int& index, int length)
    {
        while (index<length)
        {
            if (str[index] == '+' || str[index] == '-' || str[index] == '*' || str[index] == '/' || str[index] == '=' || str[index] == '%')
            {
                if (str[index + 1] == '=')
                {
                    this->oper = GetSubstring(str, index, 2);
                    index += 2;
                    this->length = 2;
                    break;
                }
                this->oper = GetSubstring(str, index, 1);
                this->length = 1;
                break;
            }
            else if (str[index == ' '])
            {
                index++;
                continue;
            }
        }
    }
};

class Expression
{
    
    
public:
    int length = 0;
    char* expressionAsString;
    Expression(char str[],int length)
    {
        expressionAsString = new char[length+1];
        for (int i = 0; i < length; i++)
        {
            expressionAsString[i] = str[i];
        }
        expressionAsString[length] = '\0';
    }
    ValueNode * valueHead = new ValueNode();
    ValueNode * valueTail;
    void Add(SomeStruct *val, OperatorNode &oper)
    {
        if (length == 0)
        {
            this->valueHead->value = val;
            valueHead->isHead = true;
            valueHead->operAfter = &oper;
            oper.valueBefore = valueHead;
            this->valueHead->isTail = true;
            valueTail = valueHead;
            length++;
        }
        else
        {
            ValueNode *temp = new ValueNode();
            temp->value = val;
            temp->operBefore = valueTail->operAfter;
            this->valueTail->isTail = false;
            this->valueTail->operAfter->valueAfter = temp;
            temp->operAfter = &oper;
            oper.valueBefore = temp;
            this->valueTail = temp;
            this->valueTail->isTail = true;
            length++;
        }
    }
    void Add(SomeStruct* val)
    {
        ValueNode* temp = new ValueNode();
        temp->value = val;
        temp->operBefore = valueTail->operAfter;
        this->valueTail->isTail = false;
        this->valueTail->operAfter->valueAfter = temp;
        this->valueTail = temp;
        this->valueTail->isTail = true;
        length++;
    }
    void MakeExpression(char str[], int length)
    {
        for (int i = 0; i < length; i++)
        {
            SomeStruct* variable = new SomeStruct();
            OperatorNode* oper = new OperatorNode();
            variable->findValue(str, i, length);
            oper->FindOperator(str, i, length);
            if (oper->length > 0)
            {
                this->Add(variable, *oper);
            }
            else
            {
                this->Add(variable);
            }
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
struct ExpressionArray
{
    Expression** arr;
    int length;
    ExpressionArray(MyList list)
    {
        this->length = list.Length();
        arr = new Expression*[length];
        for (int i = 0; i < length; i++)
        {
            Expression* ex = new Expression(list[i], list.ElementLength(i));
            ex->MakeExpression(list[i], list.ElementLength(0));
            arr[i] = ex;
        }
    }
    void GiveMeAnAnswer()
    {
        for (int i = 0; i < this->length; i++)
        {
            while (arr[i]->valueHead && arr[i])
        }

    }
    void Scale(Expression* ex)
    {

    }

};

int main()
{
    char* test1 = new char [] {"exrp"};


    MyList arr;
    GetInput(arr);
    ExpressionArray *exArr = new ExpressionArray(arr);




    for (int i = 0; i < arr.Length(); i++)
    {
        std::cout << exArr->arr[i]->expressionAsString << std::endl;
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