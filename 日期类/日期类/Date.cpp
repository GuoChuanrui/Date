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
	Date(int year = 1900, int month = 1,int day = 1)     //���캯��
		:_year(year),_month(month),_day(day)
	{
		assert(year>=1900);
		assert(month>0 && month<13);
		assert(day>0 && day<=GetMonDay(year,month));
	}
	Date(const Date& d)     //�������캯��
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}
	~Date(){}   //��������
	Date& operator= (const Date& d)    //���� =
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
		cout<<_year<<"��"<<_month<<"��"<<_day<<"��"<<endl;
	}

	Date operator+ (int NumOfDay)      //����+
	{
		Date tmp(*this);
		if(NumOfDay < 0)
		{
			tmp = tmp - (-NumOfDay);
		}
		else
		{
			int mday = GetMonDay(tmp._year,tmp._month);    //ĳ������
			while((tmp._day + NumOfDay) > mday)       //���ӹ������ȵ����������·ݼ�1
			{
				tmp._month ++;
				if(tmp._month>12)          //���·�>12����ݼ�1���·ݱ�Ϊ1��
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

	Date& operator+= (int NumOfDay)      //����+=
	{
		*this = *this + NumOfDay;  
		return *this;
	}

	Date operator- (int NumOfDay)      //����-
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
				int mday = GetMonDay(tmp._year,tmp._month);    //��������
				NumOfDay -= mday;
			}
			tmp._day -= NumOfDay;
		}
		return tmp;
	}
	
	Date& operator-= (int NumOfDay)      //����-=
	{
		*this = *this - NumOfDay;  
		return *this;
	}

	int operator- (const Date& d)   //��������������õ�����
	{
		Date tmp(d);
		int NumOfDay;
		int d_NumOfDay = GetDateNumByYMD(tmp._year,tmp._month,tmp._day);    //d_NumOfDay������d�Ǳ���ĵڼ���
		int this_NumOfDay = GetDateNumByYMD(this->_year,this->_month,this->_day);   //this_NumOfDay��this->�����Ǳ���ĵڼ���
		if(this->_year == tmp._year)    //�����ͬ�꣬�������Ǳ���ĵڼ����������ȡ����ֵ
		{
			NumOfDay = abs(d_NumOfDay - this_NumOfDay);   
		}
		else if(this->_year > tmp._year) //�����ǰ���ڴ�  
		{  
			NumOfDay = this_NumOfDay + GetDayByYear(tmp._year) - d_NumOfDay;
			while(++tmp._year < this->_year)
			{
				NumOfDay += GetDayByYear(tmp._year);
			}
		}
		else if(this->_year < tmp._year)  //�����ǰ����С
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

	Date& operator++ ()      //����++
	{
		*this += 1;
		return *this;
	}

	Date& operator++ (int )      //����++(����)
	{
		Date tmp(*this);
		*this += 1;
		return tmp;
	}

	Date& operator-- ()      //����--
	{
		*this -= 1;
		return *this;
	}

	Date& operator-- (int )      //����--(����)
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
	bool IsLeapYear(int year)    //�ж��Ƿ�����
	{
		if(((year%4==0)&&(year%100!=0))||year%400==0)
			return true;
		return false;
	}
	
	int GetDayByYear(int year)   //������ݷ���һ�������
	{
		if(IsLeapYear(year))
			return 366;
		else 
			return 365;
	}

	int GetMonDay(int year,int month)    //�������»�ȡ��������
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

	int GetDateNumByYMD(int year,int month,int day)   //���������շ�������һ��ĵڼ���
	{
		int D=0;//��һ������һ���еĵڼ���
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
	os<<d._year<<"��"<<d._month<<"��"<<d._day<<"��";
	return os;
}
istream& operator>> (istream& is,Date& d)
{
	is>>d._year>>d._month>>d._day;
	while(1)
	{
		if((d._year<1900)||(d._month<1)||(d._month>12)||(d._day<1)||(d._day>d.GetMonDay(d._year,d._month)))
		{
			cout<<"������������������"<<endl;
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
		printf("*       ��ӭʹ�����ڼ�����       *\n");
		printf("*    ��1��  ���㼸��������     *\n");
		printf("*    ��2��  �������ڲ�           *\n");
		printf("*    ��0��  ��  ��               *\n");
		printf("**********************************\n");
		printf("��ѡ��:> ");
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
			printf("�����������������룡\n");
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
	cout<<"���㼸�������ڣ�"<<endl;
	int NumOfDay;
	cout<<"������������";
	cin>>NumOfDay;
	Date tmp = d + NumOfDay;
	system("CLS");
	cout<<"��"<<d<<endl<<"���"<<NumOfDay<<"��"<<endl<<"�ǣ�"<<tmp<<endl;
}

void CalSub()
{
	system("CLS");
	Date d1(2015,12,1);
	Date d2(d1);
	cout<<"�������ڲ"<<endl;
	cout<<"������d1:"<<endl;
	cin>>d1;
	cout<<"������d2:"<<endl;
	cin>>d2;
	int NumOfDay = d1 - d2;
	system("CLS");
	cout<<d1<<endl<<"��"<<d2<<endl<<"��ࣺ"<<NumOfDay<<"��"<<endl;
}

int main()
{
	Menu();
	return 0;
}
