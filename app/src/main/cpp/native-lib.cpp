#include <jni.h>
#include <string>
#include <stack>
#include <cstring>
#include <cmath>
#include <sstream>
#include <climits>
#include <cstdio>
#include <iostream>
#pragma warning(disable:4996)
#define LIMIT_MAX  (999999999999999-1)//LONG_MAX-1
#define LIMIT_MIN  (-LIMIT_MAX)+1          //LONG_MIN+1
#define PRE 1e-12

using namespace std;

/*
 * define the numerical value of π and e
 */

const double pi = 3.1415926535897932384626433;
const double  e = 2.7182818284590452353602878;

/*
 * Unexpected response for try_catch
 */

string UN[] =
{
        "The answer is out of range.",//0
        "The denominator is zero.",//1
        "The input number is too large.", //2
        "Unexpected decimal point.", //3
        "Parentheses or operators do not match",//4
        "The number in √ is less than 0.",//5
        "The number in tan() is illegal.", //6
        "The number in log() is illegal.",//7
        "The number in asin() is illegal.",//8
        "The number in acos() is illegal.",//9
        "The number in atan() is illegal.",//10
        "The number in ln() is less than 0.",//11
        "The number in sinh() is too large.",//12
        "The number in cosh() is too large.",//13
        "The number in csc() is illegal.",//14
        "The number in sec() is illegal.",//15
        "The number in cot() is illegal.",//16
        "The exponent after ^ is too large.", //17
        "Wrong input.",//18
        "Do you forget to add '*'?",//19
        "Unknown Error."
};

/*
 * Operator precedence
 * the last row and cow of matrix is for unary operator like log(), sin()
 */

/*
 * Use follow sign to replace the operation
 * as log, sin, asin ...
 *
 * As everybody know, to calculate a math expression,
 * we should use a stack for operations and another for numbers.
 * So we also can regard function like sin(), lg(), sqrt() as operations.
 *
 * This is a table for show operations and signs one by one.
 *
 * ope      signs
 * ---------------------
 * sinh     k
 * cosh     L
 * tanh     M
 * csc      H
 * sec      I
 * cot      J
 * sin      S
 * asin     B
 * √        G
 * /        /
 * cos      C
 * acos     D
 * *        *
 * tan      T
 * atan     E
 * -        -
 * lg       A
 * +        +
 * π        p
 * e        e
 * ^        ^
 * ---------------------
 */

/*
 * Let's use a matrix to save the priority of operations.
 * 1 means that the front operation is higher that rear.
 * -1 means that the front operation is lower that rear.
 * 0 means that this situation will never exist.
 */

const int m[10][10] = {
    { 1, 1, -1,-1, -1, 1, 1, -1, -1},	//+
    { 1, 1, -1,-1, -1, 1, 1, -1, -1},	//-
    { 1, 1,  1, 1, -1, 1, 1, -1, -1},	//*
    { 1, 1,  1, 1, -1 ,1, 1, -1, -1},	// /
    {-1,-1, -1,-1, -1, 0, 9, -1, -1},	//(
    { 1, 1,  1, 1,  9, 1, 1,  1,  1},	// )
    {-1,-1, -1,-1 ,-1, 9, 0, -1, -1},	// =
    { 1, 1,  1, 1, -1, 1, 1,  1, -1},		//^
    { 1, 1,  1, 1, -1, 1, 1, -1, -1},	//A log...
};

/*
 * Here is the declaration of funtions.
 */

int Cmp(const char a, const char b);
double Cal(const double a, const char t, const double b);
void Input(stack<double>& num, stack<char>& ope, const char* t, char& ch, int& st);
double CalExp(const char* t);
bool IsSinOpe(const char ch);
double CalSin(const double a, const char t);
double Prepro(double _x);   //Precision processing
string DoubleToString(double _x);
bool ispep(const char t);
bool ispe(const char t);
void Check(const char *t);

/*
 * This function is used to calculate expression
 * MainActivity.java give native-lib.cpp a string
 * native-lib.cpp will caculate it.
 * Of course the function can return a answer as a string formate,
 * or a unexpected command for users.
 */

extern "C"
JNIEXPORT jstring JNICALL
Java_cn_edu_wust_calculator_MainActivity_Calculator_1Algorithm(JNIEnv *env, jobject instance,
                                                               jstring t_) {
    const char *t = env->GetStringUTFChars(t_, 0);
    string ans;

    try
    {
        //Check the format for expression which is deliveried.
        Check(t);

        //Give the answer to ans after process.
        ans = DoubleToString(Prepro(CalExp(t)));
    }

    catch (string unexpected)
    {
        //if something is wrong.
        ans = unexpected;
    }

    // TODO

    env->ReleaseStringUTFChars(t_, t);

    return env->NewStringUTF(ans.c_str());
}

/*
 * This function will deal with the precision problem which is lost during the caculate.
 */

double Prepro(double _x)
{
    //if _x is near 0.
    if (fabs(_x - 0.0) < PRE)
    {
        return 0;
    }

    double x = fabs(_x);
    long long int templeft = x;
    long long int tempright = templeft + 1;
    double mid = (templeft + tempright) / 2.0;

    if (x < mid && x - templeft < PRE)
    {
        //deal with precision problem.
        x = templeft;
    }

    else if (x > mid && tempright - x < PRE)
    {
        //deal with precision problem.
        x = tempright;
    }

    if (_x < 0)
    {
        //processing _x which is regarded as a positive number.
        x = -x;
    }

    return x;
}

/*
 * This function will return a string from parsing a double number.
 */

string DoubleToString(double _x)
{
    string res;
    stringstream ss;
    //20 precision setting is enough.
    ss.precision(20);

    if ((long long int)_x != _x) {
        //if _x is a double like 2.0 but we need 2
        int flag = _x < 0;
        _x = fabs(_x);

        //avoid the scientific notation caused by stringstream.
        //So first we should learn if _x is smaller than 1.
        int isunderone = _x < 1;

        if (isunderone)
            _x = _x + 1;

        int cntbit = 0;
        long long int zx = _x;

        //get the digit of _x
        do {
            cntbit++;
            zx /= 10;
        } while (zx);

        double remathpre = 0.1;

        for (int i = 0; i < 15 - cntbit; i++)
        {
            remathpre *= 0.1;
        }

        if ((long long int)_x == (long long int) (_x + remathpre))
            _x += remathpre;

        ss << _x;
        ss >> res;

        if (res.size() > 15 + 1)
        {
            res.erase(res.begin() + 16, res.end());

            for (int i = res.size() - 1; res[i] == '0'; i--)
            {
                res.erase(res.begin() + i, res.end());
            }
        }

        if (isunderone)
        {
            res[0] = '0';
        }

        if (flag)
        {
            res.insert(res.begin(), '-');
        }

        return res;
    }

    else
    {
        ss << (long long int)_x;
        ss >> res;
        return res;
    }
}

/*
 * Compareing two operation and return the higher level,
 * which is a data set by matrix m
 */

int Cmp(const char a, const char b)
{
    int i = 0, j = 0;

    switch (a)
    {
        case'+': i = 0; break;
        case'-': i = 1; break;
        case'*': i = 2; break;
        case'/': i = 3; break;
        case'(': i = 4; break;
        case')': i = 5; break;
        case'=': i = 6; break;
        case'^': i = 7; break;
        default: i = 8;		//一元运算符
    }

    switch (b)
    {
        case'+': j = 0; break;
        case'-': j = 1; break;
        case'*': j = 2; break;
        case'/': j = 3; break;
        case'(': j = 4; break;
        case')': j = 5; break;
        case'=': j = 6; break;
        case'^': j = 7; break;
        default: j = 8;		//一=一元运算符
    }

    return m[i][j];
}

/*
--------------------------------------------------------
----------FUNCTION FOR CALCULATE TWO NUMBER-------------
--------------------------------------------------------
-------------***NEED TRY CATCH(...) ORDER---------------
------------------TEST VERSION 1.0----------------------
--------------------------------------------------------
*/

double Cal(const double a, const char t, const double b) {
    double ans = 1;
    double j;
    //wrong operation throw unexpected strings.

    switch (t)
    {
        case'+':
            if (a > 0 && b > LIMIT_MAX - a) //a>0, b + a < LONG_MAX
            {
                throw UN[0];
            }

            else if (a < 0 && b < LIMIT_MIN - a) // a<0  b+a>LONG_min
            {
                throw UN[0];
            }

            ans = b + a;

            break;

        case'-':
            if (a > 0 && b < LIMIT_MIN + a)   //a>0 , b-a>min
            {
                throw UN[0];
            }

            else if (a < 0 && b > LIMIT_MAX + a)
            {
                throw UN[0];
            }

            ans = b - a;

            break;

        case'*':
            if (a > 0 && b > LIMIT_MAX / a) // a> 0 b*a<LONG_MAX
            {
                throw UN[0];
            }

            else if (a < 0 && b < LIMIT_MIN / a) // a<0 b*a>LONG_MIN
            {
                throw UN[0];
            }

            ans = b * a;

            break;

        case'/':
            if (!Prepro(a))
            {
                throw UN[1];
            }

            if (a > 0 && b > LIMIT_MAX * a)
            {
                throw UN[0];
            }

            else if (a < 0 && b < LIMIT_MIN * a)
            {
                throw UN[0];
            }

            ans = b / a;

            break;

        case'^':
            j = log(LIMIT_MAX) / log(fabs(b));

            if(fabs(b)>1&&a>j)
            {
                throw UN[17];
            }

            ans = pow(b, a);

            break;
    }

    return ans;
}

/*
--------------------------------------------------------------
---------------------FUNTION INPUT----------------------------
---------------------TEST VERSION-----------------------------
--------------------------------------------------------------
--------------SURPPORT THE EXMAMPLE AS (((1+-2)))-------------
-------------------NOT ALLOW 1----2---------------------------
--------------------------------------------------------------
*/

void Input(stack<double> & num, stack<char> & ope, const char* t, char& ch, int& st) {
    if (ch == '-' || ch == '+')
    {
        num.push(0);
        ope.push(ch);
        ch = t[++st];
    }

    if (IsSinOpe(ch))
    {
        ope.push(ch);
        ch = t[++st];
    }

    if ('0' <= ch && ch <= '9' || ch == 'p' || ch == 'e')
    {
        if (ch == 'p')
        {
            ch = t[++st];
            num.push(pi);
        }

        else if (ch == 'e')
        {
            ch = t[++st];
            num.push(e);
        }

        else
        {
            char stringnum[300];

            memset(stringnum, 0, sizeof(stringnum));
            stringnum[0] = ch;

            int i = 1;

            while ((ch = t[++st]) && '0' <= ch && ch <= '9')
            {
                stringnum[i++] = ch;
            }

            if (i > 15)  //超出输入数字大小
            {
                throw UN[2];
            }

            double sum = 0;
            long long int p = 1;
            int i2 = i;

            while (--i >= 0)
            {
                sum += (stringnum[i] - '0') * p;
                p = p * 10;
            }


            if (ch == '.')
            {
                double pp = 0.1;

                while ((ch = t[++st]) && '0' <= ch && ch <= '9' || ch == '.')
                {
                    i2++;

                    if (i2 > 15)
                    {
                        throw UN[2];
                    }

                    sum += (ch - '0') * pp;
                    pp *= 0.1;

                    if (ch == '.')
                    {
                        throw UN[3];
                    }
                }
            }

            num.push(sum);
        }
    }
}

double CalExp(const char* t)
{
    stack<double> num;
    stack<char> ope;

    ope.push('=');
    int st = -1;

    char ch;
    ch = t[++st];

    Input(num, ope, t, ch, st);

    if (ope.empty())
    {
        throw UN[4];
    }

    while (!(ch == '=' && ope.top() == '='))
    {

        if (ope.empty()) throw UN[4];
        int temp = Cmp(ope.top(), ch);
        double a, b;

        switch (temp)
        {
            case 1:
                if (ope.empty())
                {
                    throw UN[4];
                }

                if (IsSinOpe(ope.top()))
                {
                    if (num.empty())
                    {
                        throw UN[4];
                    }

                    a = num.top();

                    if (num.empty())
                    {
                        throw UN[4];
                    }

                    num.pop();

                    if (ope.empty())
                    {
                        throw UN[4];
                    }

                    num.push(CalSin(a, ope.top()));

                    if (ope.empty())
                    {
                        throw UN[4];
                    }

                    ope.pop();

                    break;
                }

                if (num.empty())
                {
                    throw UN[4];
                }

                a = num.top();

                if (num.empty())
                {
                    throw UN[4];
                }

                num.pop();

                if (num.empty())
                {
                    throw UN[4];
                }

                b = num.top();

                if (num.empty())
                {
                    throw UN[4];
                }

                num.pop();

                if (ope.empty())
                {
                    throw UN[4];
                }

                num.push(Cal(a, ope.top(), b));

                if (ope.empty())
                {
                    throw UN[4];
                }

                ope.pop();

                break;

            case 0:
                if (ope.empty())
                {
                    throw UN[4];
                }

                ope.pop();

                ch = t[++st];

                break;

            default:
                ope.push(ch);
                ch = t[++st];
                Input(num, ope, t, ch, st);
        }
    }

    return num.top();
}


bool IsSinOpe(const char ch)
{
    return ch == 'A' || ch == 'S' || ch == 'C' || ch == 'T' || ch == 'B' || ch == 'D'
    || ch == 'E' || ch == 'F' || ch == 'G' || ch == 'H' || ch == 'I' || ch == 'J' ||
    ch == 'K' || ch == 'L' || ch == 'M';
}

double CalSin(const double a, const char t)
{
    //将三角函数回归到一个周期
    double ans = 1;
    double tempa = 0;
    long long int cnt = 0;

    switch (t)
    {
        case'A':
            if (a < 0)
            {
                throw UN[7];
            }

            ans = log10(a);

            break;

        case'B':
            if (a > 1 || a < -1)
            {
                throw UN[8];
            }

            ans = asin(a);

            break;

        case'C':
            tempa = fabs(a);                //偶函数
            cnt = tempa / (2 * pi);
            tempa = a - cnt * (2 * pi);
            ans = cos(tempa);

            break;

        case'D':
            if (a > 1 || a < -1)
            {
                throw UN[9];
            }

            ans = acos(a);

            break;

        case'E':
            if (a > pi / 2 || a < -pi / 2)
            {
                throw UN[10];
            }

            ans = atan(a);

            break;

        case'F':
            if (a <= 0)
            {
                throw UN[11];
            }

            ans = log(a);

            break;

        case'G':
            if (a < 0)
            {
                throw UN[5];
            }

            ans = sqrt(a);

            break;

        case'S':
            tempa = fabs(a);                //奇函数
            cnt = tempa / (2 * pi);
            tempa = a - cnt * (2 * pi);

            if (a > 0)
            {
                ans = sin(fabs(tempa));
            }

            else
            {
                ans = -sin(fabs(tempa));
            }

            break;

        case'T':
            cnt = a / pi;
            tempa = fabs(a);            //奇函数
            cnt = tempa / pi;
            tempa = tempa - cnt * pi;

            if (fabs(tempa - pi / 2) < fabs(atan(LIMIT_MAX) - pi / 2) + PRE)
            {
                throw UN[6]; //距离铅直渐近线太近则抛出异常
            }

            if (a > 0)
            {
                ans = tan(fabs(tempa));
            }

            else
            {
                ans = -tan(fabs(tempa));
            }

            break;

        case'H': //csc
            tempa = fabs(a);                //奇函数
            cnt = tempa / (2 * pi);
            tempa = a - cnt * (2 * pi);

            if (a > 0)
            {
                tempa = sin(fabs(tempa));
            }

            else
            {
                tempa = -sin(fabs(tempa));  //先算sin
            }

            if (!Prepro(tempa) || tempa > 0 && 1 > LIMIT_MAX * tempa
            || tempa < 0 && 1 < LIMIT_MIN * tempa)
            {
                throw UN[14];
            }

            ans = 1.0 / tempa;

            break;

        case'I': //sec
            tempa = fabs(a);                //偶函数
            cnt = tempa / (2 * pi);
            tempa = a - cnt * (2 * pi);
            tempa = cos(tempa);    //先算cos

            if (!Prepro(tempa) || tempa > 0 && 1 > LIMIT_MAX * tempa
            || tempa < 0 && 1 < LIMIT_MIN * tempa)
            {
                throw UN[15];
            }

            ans = 1.0 / tempa;

            break;

        case'J': //cot
            cnt = a / pi;
            tempa = fabs(a);            //奇函数
            cnt = tempa / pi;
            tempa = tempa - cnt * pi;

            if (fabs(tempa - pi / 2) < fabs(atan(LIMIT_MAX) - pi / 2) + PRE)
            {
                throw UN[16]; //距离铅直渐近线太近则抛出异常
            }

            if (a > 0)
            {
                tempa = tan(fabs(tempa));
            }

            else
            {
                tempa = -tan(fabs(tempa));
            }

            if (!Prepro(tempa) || tempa > 0 && 1 > LIMIT_MAX * tempa
            || tempa < 0 && 1 < LIMIT_MIN * tempa)
            {
                throw UN[16];
            }

            ans = 1.0 / tempa;

            break;

        case'K':
            if (fabs(a) > 28)
            {
                throw UN[12];
            }

            ans = sinh(a);

            break;

        case'L':
            if (fabs(a) > 28)
            {
                throw UN[13];
            }

            ans = cosh(a);

            break;

        case'M':
            ans = tanh(a);

            break;
    }

    return ans;
}

bool ispep(const char t)
{
    return t=='.'||t=='p'||t=='e';
}

bool ispe(const char t)
{
    return t=='p'||t=='e';
}

void Check(const char *t)
{
    for(int i= 0;i<strlen(t)-2;i++)
    {
        if(ispep(t[i])&&ispep(t[i+1])||'0'<=t[i]&&t[i]<='9'&&ispe(t[i+1])
        ||'0'<=t[i+1]&&t[i+1]<='9'&&ispe(t[i]))
        {
            throw UN[18];
        }

        if(ispep(t[i])&&IsSinOpe(t[i+1])||'0'<=t[i]&&t[i]<='9'&&IsSinOpe(t[i+1]))
        {
            throw UN[19];
        }
    }
}
