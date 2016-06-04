#include<iostream>
#include<assert.h>
using namespace std;
class Date
{
public: 
	friend void GetDateAfterDay();
	friend void CalSub();
	friend istream& operator>> (istream& is,Date& d);
	friend ostream& operator<< (ostream& os,const Date& d);
	Date(int year = 1900, int month = 1,int day = 1)     //构造函数
		:_year(year),_month(month),_day(day)
	{
		assert(year>=1900);
		assert(month>0 && month<13);
		assert(day>0 && day<=GetMonDay(year,month));
	}
	Date(const Date& d)     //拷贝构造函数
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}
	~Date(){}   //析构函数
	Date& operator= (const Date& d)    //重载 =
	{
		if(this != &d)
		{
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}
		return *this;
	}
	void Display()    
	{
		cout<<_year<<"年"<<_month<<"月"<<_day<<"日"<<endl;
	}

	Date operator+ (int NumOfDay)      //重载+
	{
		Date tmp(*this);
		if(NumOfDay < 0)
		{
			tmp = tmp - (-NumOfDay);
		}
		else
		{
			int mday = GetMonDay(tmp._year,tmp._month);    //某月天数
			while((tmp._day + NumOfDay) > mday)       //当加过天数比当月天数大，月份加1
			{
				tmp._month ++;
				if(tmp._month>12)          //当月份>12，年份加1，月份变为1月
				{
					tmp._year ++;
					tmp._month = 1;
				}
				NumOfDay -= mday;         
				mday=GetMonDay(tmp._year,tmp._month);
			}
			tmp._day += NumOfDay;
		}
		return tmp;
	}

	Date& operator+= (int NumOfDay)      //重载+=
	{
		*this = *this + NumOfDay;  
		return *this;
	}

	Date operator- (int NumOfDay)      //重载-
	{
		Date tmp(*this);
		if(NumOfDay < 0)
		{
			tmp = tmp + (-NumOfDay);
		}
		else
		{
			while((tmp._day - NumOfDay) <= 0)
			{
				tmp._month --;
				if(tmp._month < 1)
				{
					tmp._year --;
					tmp._month = 12;
				}
				int mday = GetMonDay(tmp._year,tmp._month);    //上月天数
				NumOfDay -= mday;
			}
			tmp._day -= NumOfDay;
		}
		return tmp;
	}
	
	Date& operator-= (int NumOfDay)      //重载-=
	{
		*this = *this - NumOfDay;  
		return *this;
	}

	int operator- (const Date& d)   //两个日期相减，得到天数
	{
		Date tmp(d);
		int NumOfDay;
		int d_NumOfDay = GetDateNumByYMD(tmp._year,tmp._month,tmp._day);    //d_NumOfDay：日期d是本年的第几天
		int this_NumOfDay = GetDateNumByYMD(this->_year,this->_month,this->_day);   //this_NumOfDay：this->日期是本年的第几天
		if(this->_year == tmp._year)    //如果是同年，用日期是本年的第几天天数相减取绝对值
		{
			NumOfDay = abs(d_NumOfDay - this_NumOfDay);   
		}
		else if(this->_year > tmp._year) //如果当前日期大  
		{  
			NumOfDay = this_NumOfDay + GetDayByYear(tmp._year) - d_NumOfDay;
			while(++tmp._year < this->_year)
			{
				NumOfDay += GetDayByYear(tmp._year);
			}
		}
		else if(this->_year < tmp._year)  //如果当前日期小
		{
			NumOfDay=GetDayByYear(this->_year) - this_NumOfDay + d_NumOfDay;
			while(++this->_year < tmp._year)
			{
				NumOfDay += GetDayByYear(this->_year);  
			}
		}
		else
		{
			return 0;
		}
		return NumOfDay;
	}

	Date& operator++ ()      //重载++
	{
		*this += 1;
		return *this;
	}

	Date& operator++ (int )      //重载++(后置)
	{
		Date tmp(*this);
		*this += 1;
		return tmp;
	}

	Date& operator-- ()      //重载--
	{
		*this -= 1;
		return *this;
	}

	Date& operator-- (int )      //重载--(后置)
	{
		Date tmp(*this);
		*this -= 1;
		return tmp;
	}

	bool operator> (const Date& d)
	{
		if(this->_year > d._year)
			return true;
		else if(this->_year == d._year)
		{
			if(this->_month > d._month)
				return true;
			else if(this->_month == d._month)
			{
				if(this->_day > d._day)
					return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	bool operator== (const Date& d)
	{
		return _year==d._year && _month==d._month && _day==d._day;
	}

	bool operator>= (const Date& d)
	{
		return *this>d || *this==d;
	}

	bool operator< (const Date& d)
	{
		return !(*this>d || *this==d);
	}

	bool operator<= (const Date& d)
	{
		return *this<d || *this==d;
	}

private:
	bool IsLeapYear(int year)    //判断是否闰年
	{
		if(((year%4==0)&&(year%100!=0))||year%400==0)
			return true;
		return false;
	}
	
	int GetDayByYear(int year)   //根据年份返回一年的天数
	{
		if(IsLeapYear(year))
			return 366;
		else 
			return 365;
	}

	int GetMonDay(int year,int month)    //根据年月获取当月天数
	{
		assert(year>=1900);
		assert(month>0 && month<13);
		static int monthArray[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
		int day=monthArray[month];
		if((month == 2) && IsLeapYear(year))
		{
			day+=1;
		}
		return day;
	}

	int GetDateNumByYMD(int year,int month,int day)   //根据年月日返回是这一年的第几天
	{
		int D=0;//这一天在这一年中的第几天
		for(int i=1;i<month;i++)
		{
			D+=GetMonDay(year,i);
		}
		D+=day;
		return D;
	}
	
private:
	int _year;
	int _month;
	int _day;
};

ostream& operator<< (ostream& os,const Date& d)
{
	os<<d._year<<"年"<<d._month<<"月"<<d._day<<"日";
	return os;
}
istream& operator>> (istream& is,Date& d)
{
	is>>d._year>>d._month>>d._day;
	while(1)
	{
		if((d._year<1900)||(d._month<1)||(d._month>12)||(d._day<1)||(d._day>d.GetMonDay(d._year,d._month)))
		{
			cout<<"输入有误，请重新输入"<<endl;
			is>>d._year>>d._month>>d._day;
		}
		else
			break;
	}
	return is;
}

void Menu()
{
	int select=1;
	while(select)
	{
		system("CLS");
		printf("**********************************\n");
		printf("*       欢迎使用日期计算器       *\n");
		printf("*    【1】  推算几天后的日期     *\n");
		printf("*    【2】  计算日期差           *\n");
		printf("*    【0】  退  出               *\n");
		printf("**********************************\n");
		printf("请选择:> ");
		cin>>select;
		switch(select)
		{
		case 1:
			GetDateAfterDay();
			break;
		case 2:
			CalSub();
			break;
		case 0:
			exit(0);
			break;
		default:
			printf("输入有误，请重新输入！\n");
			system("pause");
			break;
		}
		system("pause");
		Menu();
	}
}

void GetDateAfterDay()
{
	system("CLS");
	Date d(2015,12,1);
	cout<<"推算几天后的日期："<<endl;
	int NumOfDay;
	cout<<"请输入天数：";
	cin>>NumOfDay;
	Date tmp = d + NumOfDay;
	system("CLS");
	cout<<"和"<<d<<endl<<"相距"<<NumOfDay<<"天"<<endl<<"是："<<tmp<<endl;
}

void CalSub()
{
	system("CLS");
	Date d1(2015,12,1);
	Date d2(d1);
	cout<<"计算日期差："<<endl;
	cout<<"请输入d1:"<<endl;
	cin>>d1;
	cout<<"请输入d2:"<<endl;
	cin>>d2;
	int NumOfDay = d1 - d2;
	system("CLS");
	cout<<d1<<endl<<"和"<<d2<<endl<<"相距："<<NumOfDay<<"天"<<endl;
}

int main()
{
	Menu();
	return 0;
}
