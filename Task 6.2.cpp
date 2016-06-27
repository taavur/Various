/*
 
 AUTHOR:        Taavi Kappak
 
 TASK:          Create HugeInt type, a signed integer number of 256 bit size that supports
				the 4 basic operations (+, *, -, /), modulus, and all the comparison operators 
				(==, !=, >, …).

 DATE:          15.07.2014
 
 */

#include <iostream>

using std::cout;
using std::cin;
using std::endl;

const int size = 256;

class hugeIntClass {
private:
	int integer[size], sign;
	int length() const {
		int temp = 0;
		while ((temp < size) && (integer[temp] == 0 ))
			temp++;
		return (size - temp);       
	}
public:
	void resetToZero() {
		sign = 1;
		for(int i = 0; i < size; i++) {
			integer[i] = 0;
		}
	}
	hugeIntClass() {
		resetToZero();
	}
	hugeIntClass(long number) {
		resetToZero();
		if (number < 0) {
			sign = -1;
			number = -number;
		}
		for (int i = size - 1; number != 0; i--) {
			integer[i] = number % 10;
			number = number / 10;
		}
	}
	bool hugeIntClass::isLessThan(const hugeIntClass & other) const {
		for (int i = 0; i < size; i++) {
			if (integer[i] < other.integer[i]) {
				return true;
			}
			else if (integer[i] > other.integer[i]) {
				return false;
			}
		}
		return false;
	}
	hugeIntClass addingNumbers(const hugeIntClass & other) const {
		hugeIntClass result = *this;
		int carry = 0;
		for (int i = size - 1; i >= 0; i--) {
			int res = integer[i] + other.integer[i] + carry;
			result.integer[i] = res % 10;
			carry = res / 10;
		}
		return result;
	}
	hugeIntClass substractingNumbers(const hugeIntClass & other) const {
		hugeIntClass temp;
		temp = *this;
		hugeIntClass result;
		for (int i = size -1; i > 0; i--) {
			if (temp.integer[i] < other.integer[i] ) {
				temp.integer[i-1]--;
				temp.integer[i] += 10;
			}    
			result.integer[i] = temp.integer[i] - other.integer[i];
		}
		return result;
	}
	hugeIntClass operator+ (long number) const {
		hugeIntClass result, temp(number);
		result = *this + temp;
		return result;
	}
	hugeIntClass operator+ (const hugeIntClass & other) const {
		hugeIntClass firstNumber = *this;
		hugeIntClass SecondNumber = other;
		hugeIntClass result;
		if (sign * other.sign == 1) {
			result = addingNumbers(other);
			result.sign = sign;
			return result;
		}
		if ( firstNumber.isLessThan(SecondNumber) ) {
			result = SecondNumber.substractingNumbers(firstNumber);
			result.sign = SecondNumber.sign;
			return result;
		}
		if (SecondNumber.isLessThan(firstNumber) ) {
			result = firstNumber.substractingNumbers(SecondNumber);
			result.sign = firstNumber.sign;
		}
		return result;
	}
	hugeIntClass operator* (int number) const {
		hugeIntClass result;
		int carry = 0, subResult;
		for (int i = size - 1; i >= 1; i--) {
			subResult = integer[i] * number + carry;
			result.integer[i] = subResult % 10;
			carry = subResult / 10;
		}
		result.sign = sign;
		return result;
	}
	hugeIntClass operator* (const hugeIntClass & other) const {
		hugeIntClass result;
		hugeIntClass temp;
		int carry = 0, someTemp = 0, k = 0;
		for (int i = size - 1; i > size/2; i--) {
			carry = 0;
			temp.resetToZero();
			for (int j = size - 1; j > size/2; j--) {
				someTemp = integer[i] * other.integer[j] + carry;
				temp.integer[j-k] = someTemp % 10;
				carry = someTemp / 10;
			}  
			k++;   
			result = result + temp;
		}  
		result.sign = sign * other.sign;
		return result;   
	}
	hugeIntClass operator- (const hugeIntClass & other) const {
		hugeIntClass result = *this;
		hugeIntClass another = other;
		another.sign = -another.sign;
		result = *this + another;
		return result;
	}
	hugeIntClass operator/ (const hugeIntClass & other) const {
		hugeIntClass result, temp;
		if (other == result ) {
			_ASSERT_EXPR(false, L"Dividing by zero. Exiting.");
			cout << "Dividing by zero. Exiting.";
			exit (1);
		}
		hugeIntClass varI = *this, varJ = other, varT;
		int origSignForThis = varI.sign, origSignForOther = other.sign;
		varI.sign = 1;
		varJ.sign = 1;
		if (varI < varJ) {
			return result;
		}
		int x = varI.length(), y = varJ.length(), p1 = size - x, p2 = size - y, n = 0;
		for (int j = 0; j < y; j++) {
			varT.integer[size - y + j] = varI.integer[p1 + j];
		}
		for (int i = p1 + y; i < size; i++) {
			n = 0;
			while (varJ * n <= varT) {
				n++;
			}
			n--; 
			result.integer[i - 1] = n;
			temp =  varT - varJ * n;
			varT.resetToZero();
			for(int j = size - temp.length(); j < size; j++) {
				varT.integer[j - 1] = temp.integer[j];
			}
			varT.integer[size - 1] = varI.integer[i];
		}
		n = 0;
		while (varJ * n <= varT) {
			n++;
		}
		n--;
		result.integer[size - 1] = n;
		result.sign = origSignForThis * origSignForOther;
		return result;
	}
	hugeIntClass operator% (const hugeIntClass & other) const {
		hugeIntClass firstVar = *this, secondVar = other, temp;
		firstVar.sign = 1;
		secondVar.sign = 1;
		temp = firstVar / secondVar;
		hugeIntClass resultVar = firstVar - (temp * secondVar);
		resultVar.sign = sign;
		return resultVar;
	}
	bool operator== (const hugeIntClass & other) const {
		if (sign * other.sign == -1)
			return false;
		for (int i = 0; i < size; i++)
			if (integer[i] != other.integer[i]) {
				return false;
			}
		return true;
	}
	bool operator!= (const hugeIntClass & other) const {
		return !(*this == other);
	}
	bool operator> (const hugeIntClass & other) const {
		return (!(*this < other) && (*this != other));
	}
	bool operator< (const hugeIntClass & other) const {
		hugeIntClass first = *this;
		hugeIntClass second = other;
		if ((sign == 1) && (other.sign == -1))
			return false;
		if ((sign == -1 ) && (other.sign == 1))
			return true;
		if (first == second)
			return false;
		if ((first.isLessThan(second)) && (sign == 1))
			return true;
		return false;
	}
	bool operator>= (const hugeIntClass & other)const {
		return ((*this > other) || (*this == other));
	}
	bool operator<= (const hugeIntClass & other) const {
		return ((*this < other) || (*this == other));
	}
};

int main() {

	hugeIntClass firstvar = 12509, secondvar = 0, temp;
	int pizza = 0;
	temp = secondvar * firstvar;
	temp = 0;
	temp = firstvar / secondvar;
	temp = 0;
	temp = secondvar + firstvar;
	temp = 0;
	temp = firstvar - secondvar;
	temp = 0;
	temp = secondvar % firstvar;
	temp = 0;
	temp = firstvar % secondvar;


	if (secondvar == firstvar)
		pizza = 6;
	if (secondvar < firstvar)
		pizza = 1;
	if (secondvar > firstvar)
		pizza = 2;
	if (secondvar <= firstvar)
		pizza = 3;
	if (secondvar >= firstvar)
		pizza = 4;
	if (secondvar != firstvar)
		pizza = 5;


	return 0;
}