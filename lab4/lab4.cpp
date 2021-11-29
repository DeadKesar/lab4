//TODO:
//Разработать программу - калькулятор, которая способна сохранять любое количество именованных пользовательских переменных и использовать их для расчета простейших алгебраических выражений.Предусмотреть возможность определения любой из переменных простейшим алгебраическим выражением.Например:
//exrp = first - second + third
//someRes = expr + 10
//first = 10
//Предусмотреть возможность вывода любой переменной на экран.Если есть выражения, переменные которых не определены, то пользователю выводится это выражение и предлагается ввести все необходимые значения переменных с консоли.

//ИДБ-21-12 Порядин Вячеслав Сергеевич. 
#include <iostream>
#include <sstream>

struct ValueStruct;
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
double GetNumber(const char s[]);
bool Compare(const char st1[], int len1, const char st2[], int len2);

struct ValueStruct
{
    int openBrackets = 0;
    int closeBrackets = 0;
    bool hasValue = false;
    bool isMinus = false;
    char* variableName;
    int nameLength;
    int numberOfUsing = 0; //количество использований
    double value;
    ValueStruct()
    {   }
    ValueStruct(char name[], int nameLength, double value)
    {
        this->variableName = name;
        this->nameLength = nameLength;
        this->value = value;
    }
    ~ValueStruct()
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
        while ((str[index] == '(' || str[index] == ' ' )&& index < length)
        {
            if (str[index] == '(')
            {
                this->openBrackets++;
            }
            index++;
        }
        bool isWord = false;
        while (index < length)
        {
            if (!(isWord || str[index] == ' '))
            {
                startWord = index;
                isWord = true;
            }
            if (isWord && (str[index] == ' ' || str[index] == '+' || str[index] == '-' || str[index] == '*' || str[index] == '/' || str[index] == '=' || str[index] == '%'|| str[index] == '('|| str[index] == ')'))
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
        while ((str[index] == ')' || str[index] == ' ') && index < length)
        {
            if (str[index] == ')')
            {
                this->closeBrackets++;
            }
            index++;
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
    ValueStruct *value;
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
    bool compair(const char str[], int length)
    {
        if (this->length == length)
        {
            for (int i = 0; i < length; i++)
            {
                if (str[i] != this->oper[i])
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};
class Expression
{   
public:
    int length = 0;
    char* expressionAsString;
    Expression(char str[],int strLength)
    {
        expressionAsString = new char[strLength+1];
        for (int i = 0; i < strLength; i++)
        {
            expressionAsString[i] = str[i];
        }
        expressionAsString[strLength] = '\0';
    }
    ValueNode * valueHead = new ValueNode();
    ValueNode * valueTail;
    void Add(ValueStruct *val, OperatorNode &oper)
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
        if (this->length >=3 && this->valueTail->operBefore->compair("-", 1) && this->valueTail->operBefore->valueBefore->value->nameLength == 0)
        {
            this->valueTail->operBefore = this->valueTail->operBefore->valueBefore->operBefore;
            delete this->valueTail->operBefore->valueAfter->operAfter;
            delete this->valueTail->operBefore->valueAfter;
            valueTail->operBefore->valueAfter = this->valueTail;
            valueTail->value->isMinus = true;
            length--;
        }

    }
    void Add(ValueStruct* val)
    {
        ValueNode* temp = new ValueNode();
        temp->value = val;
        if (valueTail)
        {
            temp->operBefore = valueTail->operAfter;
            this->valueTail->isTail = false;
            this->valueTail->operAfter->valueAfter = temp;
            this->valueTail = temp;
            this->valueTail->isTail = true;
        }
        else
        {
            valueTail = temp;
            valueHead = valueTail;
            
            length -= 10;
        }
        length++;
        if (this->length >= 3 && this->valueTail->operBefore->compair("-", 1) && this->valueTail->operBefore->valueBefore->value->nameLength == 0)
        {
            this->valueTail->operBefore = this->valueTail->operBefore->valueBefore->operBefore;
            delete this->valueTail->operBefore->valueAfter->operAfter;
            delete this->valueTail->operBefore->valueAfter;
            valueTail->operBefore->valueAfter = this->valueTail;
            valueTail->value->isMinus = true;
            length--;
        }
    }
    void MakeExpression(char str[], int length)
    {
        for (int i = 0; i < length; i++)
        {
            ValueStruct* variable = new ValueStruct();
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
            if (this->valueTail->value->isMinus)
            {
                this->valueTail->value->value *= (-1);
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
            ex->MakeExpression(list[i], list.ElementLength(i));
            arr[i] = ex;
        }
    }
    void GiveMeAnAnswers()
    {
        for (int i = 0; i < this->length; i++)
        {
            if (!(this->arr[i]->valueHead->isHead && this->arr[i]->valueHead->isTail))
            {
                if (arr[i]->valueHead->value->hasValue)
                {
                    continue;
                }
                if (arr[i]->length < 0)
                {
                    AskValue(arr[i]->valueHead->value, i);
                    continue;
                }
                ScaleByOperator(this->arr[i]->valueHead, i);
            }
        }

    }
    void ScaleByOperator(ValueNode* val, int positionInArray)
    {
        //если голова и хвост не совпадают и есть хотя бы один оператор
        if (!(val->isHead && val->isTail) && val->operAfter)
        {
            //если мы в начале но ещё не конец
            if (val->isHead && !val->operAfter->valueAfter->isTail)
            {
                    ScaleByOperator(val->operAfter->valueAfter, positionInArray);
            }
            //пока не голва или хвост 
            while (!(val->isTail || val->isHead))
            {
                if (val->operAfter->valueAfter->value->openBrackets != 0)
                {
                    ScaleByOperator(val->operAfter->valueAfter, positionInArray);
                }
                if (!val->isTail    && !val->operAfter->valueAfter->isTail)
                {
                    if ((val->operAfter->valueAfter->value->closeBrackets == 0) &&
                        (val->operAfter->valueAfter->operAfter->compair("*", 1) || val->operAfter->valueAfter->operAfter->compair("*=", 2) || val->operAfter->valueAfter->operAfter->compair("/", 1)
                            || val->operAfter->valueAfter->operAfter->compair("/=", 2) || val->operAfter->valueAfter->operAfter->compair("%", 1) || val->operAfter->valueAfter->operAfter->compair("%=", 2)))
                    {
                        ScaleByOperator(val->operAfter->valueAfter, positionInArray);
                    }
                }
                if (!(val->value->closeBrackets != 0))
                {
                    Scale(val, positionInArray);
                }
            }
            if (val->isHead && !val->isTail && val->operAfter->valueAfter->isTail)
            {
                DefinitValueHead(val, positionInArray);
            }
        }

    }
    void DefinitValueHead(ValueNode* val, int positionInArray)
    {
        if (!val->operAfter->valueAfter->value->hasValue)
        {
            FindValueInExpressionAndMakeIt(val->operAfter->valueAfter->value, positionInArray);
        }
        if (val->operAfter->compair("=",1))
        {
            val->value->value = val->operAfter->valueAfter->value->value;
            delete val->operAfter->valueAfter;
            delete val->operAfter;
            arr[positionInArray]->valueTail = val;
            arr[positionInArray]->length--;
        }
        else
        {
            if (!val->value->hasValue)
            {
                FindValueInExpressionAndMakeIt(val->value, positionInArray);
            }
            if (val->operAfter->compair("+=",2))
            {
                val->value->value += val->operAfter->valueAfter->value->value;
                delete val->operAfter->valueAfter;
                delete val->operAfter;
                arr[positionInArray]->valueTail = val;
                arr[positionInArray]->length--;
            }
            if (val->operAfter->compair("-=", 2))
            {
                val->value->value -= val->operAfter->valueAfter->value->value;
                delete val->operAfter->valueAfter;
                delete val->operAfter;
                arr[positionInArray]->valueTail = val;
                arr[positionInArray]->length--;
            }
            if (val->operAfter->compair("*=", 2))
            {
                val->value->value *= val->operAfter->valueAfter->value->value;
                delete val->operAfter->valueAfter;
                delete val->operAfter;
                arr[positionInArray]->valueTail = val;
                arr[positionInArray]->length--;
            }
            if (val->operAfter->compair("/=", 2))
            {
                val->value->value /= val->operAfter->valueAfter->value->value;
                delete val->operAfter->valueAfter;
                delete val->operAfter;
                arr[positionInArray]->valueTail = val;
                arr[positionInArray]->length--;
            }
            if (val->operAfter->compair("&=", 2))
            {
                val->value->value = val->value->value - (int)val->value->value + (int)val->value->value % (int)val->operAfter->valueAfter->value->value;
                delete val->operAfter->valueAfter;
                delete val->operAfter;
                arr[positionInArray]->valueTail = val;
                arr[positionInArray]->length--;
            }
        }
        val->isTail = true;
        arr[positionInArray]->valueTail->value->hasValue = true;
    }
    void Scale(ValueNode* value, int positionInArray)
    {
        bool isBrackets = false;
        if (!value->isTail)
        {
            //находим значения для операндов
            if (!value->value->hasValue)
            {
                FindValueInExpressionAndMakeIt(value->value, positionInArray);
            }
            if (!value->operAfter->valueAfter->value->hasValue)
            {
                FindValueInExpressionAndMakeIt(value->operAfter->valueAfter->value, positionInArray);
            }

            if (value->operAfter->valueAfter->value->closeBrackets != 0)
            {
                if (value->value->openBrackets > value->operAfter->valueAfter->value->closeBrackets)
                {
                    value->value->openBrackets -= value->operAfter->valueAfter->value->closeBrackets;
                }
                else
                {
                    value->operAfter->valueAfter->value->closeBrackets -= value->value->openBrackets;
                    value->value->openBrackets = 0;
                    value->value->closeBrackets = value->operAfter->valueAfter->value->closeBrackets;
                    isBrackets = true;
                }
            }
            if (value->operAfter->compair("+",1) || value->operAfter->compair("+=",2))
            {
                value->value->value += value->operAfter->valueAfter->value->value;
            }
            else if (value->operAfter->compair("-",1) || value->operAfter->compair("-=",2))
            {
                value->value->value -= value->operAfter->valueAfter->value->value;
            }
            else if (value->operAfter->compair("*",1) || value->operAfter->compair("*=",2))
            {
                value->value->value *= value->operAfter->valueAfter->value->value;
            }
            else if (value->operAfter->compair("/",1) || value->operAfter->compair("/=",2))
            {
                value->value->value /= value->operAfter->valueAfter->value->value;
            }
            else if (value->operAfter->compair("%",1) || value->operAfter->compair("%=",2))
            {
                value->value->value = value->value->value - (int)value->value->value + (int)value->value->value % (int)value->operAfter->valueAfter->value->value;
            }
            if (!value->operAfter->valueAfter->isTail)
            {
                DeleteNodeWithOperator(value,positionInArray);
            }
            else
            {
                value->isTail = true;
                delete value->operAfter->valueAfter;
                delete value->operAfter;
            }
            if (isBrackets && !value->operBefore->valueBefore->isHead)
            {
                ScaleByOperator(value->operBefore->valueBefore, positionInArray);
            }
        }
    } 
    void DeleteNodeWithOperator(ValueNode* value, int positionInArray)
    {
        value->operAfter = value->operAfter->valueAfter->operAfter;
        delete value->operAfter->valueAfter->operBefore->valueBefore->operBefore;
        delete value->operAfter->valueAfter->operBefore->valueBefore;
        value->operAfter->valueAfter->operBefore = value->operAfter;
        value->operAfter->valueBefore = value;
        arr[positionInArray]->length--;
    }
    void AskValue(ValueStruct* value, int positionInArray)
    {
        std::cout << "В выражении: " << this->arr[positionInArray]->expressionAsString << std::endl << "для переменной \"" << 
                    value->variableName  << "\" значение для выражения не определенно " << std::endl;
        value->value = GetNumber(value->variableName);
        if (value->isMinus)
        {
            value->value *= (-1.0);
        }
        value->hasValue = true;
    }
    void FindValueInExpressionAndMakeIt(ValueStruct* value,int positionInArray)
    {                
        for (int i = positionInArray +1; i < this->length; i++)
        {
            if (Compare(this->arr[i]->valueHead->value->variableName, this->arr[i]->valueHead->value->nameLength, value->variableName, value->nameLength) && this->arr[i]->valueHead->value->hasValue)
            {
                value->value = this->arr[i]->valueHead->value->value;
                if (value->isMinus)
                {
                    value->value *= (-1.0);
                }
                this->arr[i]->valueHead->value->numberOfUsing++;
                value->hasValue = true;
                return;
            }
            else if (Compare(this->arr[i]->valueHead->value->variableName, this->arr[i]->valueHead->value->nameLength, value->variableName, value->nameLength))
            {
                ScaleByOperator(this->arr[i]->valueHead, i);
                if (!arr[i]->valueHead->value->hasValue)
                    AskValue(arr[i]->valueHead->value, i);
                value->value = this->arr[i]->valueHead->value->value;
                this->arr[i]->valueHead->value->numberOfUsing++;
                value->hasValue = true;
                return;
            }
        }
        for (int i = positionInArray-1; i >= 0; i--)
        {
            if (Compare(this->arr[i]->valueHead->value->variableName, this->arr[i]->valueHead->value->nameLength, value->variableName, value->nameLength) && this->arr[i]->valueHead->value->hasValue)
            {
                value->value = this->arr[i]->valueHead->value->value;
                if (value->isMinus)
                {
                    value->value *= (-1.0);
                }
                this->arr[i]->valueHead->value->numberOfUsing++;
                value->hasValue = true;
                return;
            }
            else if (Compare(this->arr[i]->valueHead->value->variableName, this->arr[i]->valueHead->value->nameLength, value->variableName, value->nameLength))
            {
                ScaleByOperator(this->arr[i]->valueHead, i);
                if (!arr[i]->valueHead->value->hasValue)
                    AskValue(arr[i]->valueHead->value, i);
                value->value = this->arr[i]->valueHead->value->value;
                this->arr[i]->valueHead->value->numberOfUsing++;
                value->hasValue = true;
                return;
            }
        }
        AskValue(value, positionInArray);
    }
    void PrintAnswers()
    {
        for (int i = 0; i < this->length; i++)
        {
            if (this->arr[i]->valueHead->value->numberOfUsing == 0)
            {
                std::cout << "значение для выражения " << this->arr[i]->expressionAsString <<
                    std::endl << "= " << this->arr[i]->valueHead->value->value << std::endl;
            }
        }
    }
};

int main()
{
    setlocale(LC_ALL, "Rus");
    std::cout << "Лабораторная работа №4 задание 5*" << std::endl << "Введите выражение для расчёта. Программа принимает строки как выражения. "
        << std::endl << "пример: " << std::endl <<
        "answer = a *(10 +b) - -5" << std::endl <<
        "b = 4" << std::endl <<
        "a = -1" << std::endl <<
        "................." << std::endl << "Для завершения ввода, введите сочетание клавишь ctrl + z." << std::endl;
    std::cout << "Введите выражение: " << std::endl;



    MyList arr;
    GetInput(arr);
    ExpressionArray *exArr = new ExpressionArray(arr);
    exArr->GiveMeAnAnswers();
    exArr->PrintAnswers();

    delete exArr;
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
double GetNumber(const char s[])
{
    double num;
    while (true)
    {
        std::cout << s << " = ";
        std::cin >> num;
        if (std::cin.fail()) //проверяем, не заблокирован ли буфер ввода, если да, значит значения ввода юыло некоректным.
        {
            std::cin.clear(); //возвращаем буфер в обычный режим
            std::cin.ignore(32767, '\n'); // очищаем буфер ввода
            std::cout << "некоректное значение, попробуйте снова" << std::endl; //просим повторный ввод
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очищаем буфер ввода от лишних символов, если таковые есть
            return num; // выходим из функции.
        }
    }
}
bool Compare(const char st1[], int len1, const char st2[], int len2)
{
    if (len1 == len2)
    {
        for (int i = 0; i < len1; i++)
        {
            if (st1[i] != st2[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}