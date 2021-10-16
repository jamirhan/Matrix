#include <iostream>
#include <vector> 
#include <string>
#include <algorithm>
#include <exception>

int pow(const int a) {
	int ans = 1;
	for (int i = 0; i < a; ++i) ans *= 10;
	return ans;
}

double eps = 1e-6;

static const int base = 4;

class BigInteger {
	bool is_neg = false;
	std::vector<int> number; //reversed
	int dec = pow(base);
	int last_val = dec - 1;

	bool abs_less(const BigInteger& second_val) const {
		if (number.size() < second_val.number.size()) return true;
		else if (number.size() > second_val.number.size()) return false;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return number[iter_1] < second_val.number[iter_1];
			--iter_1;
		}
		return false;
	}

	BigInteger& operate_minus(const BigInteger& second_val) {
		if (abs_less(second_val)) {
			is_neg = true;
			int ost = 0;
			size_t iter_1 = 0;
			size_t iter_2 = 0;
			int last_zero_value = -2;
			while (iter_1 < number.size() && iter_2 < second_val.number.size()) {
				int digit = second_val.number[iter_2] - number[iter_1] - ost;
				if (digit < 0) {
					digit += dec;
					ost = 1;
				}
				else {
					ost = 0;
				}
				if (digit == 0 && last_zero_value == -2) last_zero_value = iter_1;
				else if (digit != 0) last_zero_value = -2;
				number[iter_1] = digit;
				++iter_1;
				++iter_2;
			}

			while (iter_2 < second_val.number.size()) {
				int digit = second_val.number[iter_2] - ost;
				if (digit < 0) {
					digit += dec;
					ost = 1;
				}
				else {
					ost = 0;
				}
				if (digit == 0 && last_zero_value == -2) last_zero_value = iter_2;
				else if (digit != 0) last_zero_value = -2;
				number.push_back(digit);
				++iter_2;
			}
			if (last_zero_value != -2 && last_zero_value != 0) {
				number.resize(last_zero_value);
			}
			else if (last_zero_value == 0) number.resize(1);
			return *this;
		}
		int ost = 0;
		size_t iter_1 = 0;
		size_t iter_2 = 0;
		int last_zero_value = -2;
		while (iter_1 < number.size() && iter_2 < second_val.number.size()) {
			int digit = number[iter_1] - second_val.number[iter_2] - ost;
			if (digit < 0) {
				digit += dec;
				ost = 1;
			}
			else {
				ost = 0;
			}
			if (digit == 0 && last_zero_value == -2) last_zero_value = iter_1;
			else if (digit != 0) last_zero_value = -2;
			number[iter_1] = digit;
			++iter_1;
			++iter_2;
		}

		while (iter_1 < number.size()) {
			int digit = number[iter_1] - ost;
			if (digit < 0) {
				digit += dec;
				ost = 1;
			}
			else {
				ost = 0;
			}
			if (digit == 0 && last_zero_value == -2) last_zero_value = iter_1;
			else if (digit != 0) last_zero_value = -2;
			number[iter_1] = digit;
			++iter_1;
		}
		if (last_zero_value != -2 && last_zero_value != 0) {
			number.resize(last_zero_value);
		}
		else if (last_zero_value == 0) number.resize(1);
		return *this;
	}

	BigInteger& operate_plus(const BigInteger& second_val) {

		int ost = 0;
		size_t iter_1 = 0;
		size_t iter_2 = 0;

		while (iter_1 < number.size() && iter_2 < second_val.number.size()) {
			int digit = number[iter_1] + second_val.number[iter_2] + ost;
			ost = digit / dec;
			number[iter_1] = digit % dec;
			++iter_1;
			++iter_2;
		}

		while (iter_1 < number.size()) {
			int digit = number[iter_1] + ost;
			ost = digit / dec;
			number[iter_1] = digit % dec;
			++iter_1;
		}

		while (iter_2 < second_val.number.size()) {
			int digit = second_val.number[iter_2] + ost;
			ost = digit / dec;
			number.push_back(digit % dec);
			++iter_2;
		}

		if (ost != 0)
			number.push_back(ost);

		return *this;
	}

	BigInteger& operate_double_plus() {
		int ost = 1;
		size_t it = 0;
		while (ost != 0 && it < number.size()) {
			if (number[it] != last_val) {
				++number[it];
				ost = 0;
			}
			else {
				number[it] = 0;
				ost = 1;
			}
			++it;
		}
		if (it == number.size() && ost != 0)
			number.push_back(1);
		return *this;
	}

	BigInteger& operate_double_minus() {
		if (number.size() == 1 && number[0] == 1) {
			is_neg = false;
			number[0] = 0;
			return *this;
		}
		int ost = 1;
		size_t it = 0;
		while (ost != 0 && it < number.size()) {
			if (number[it] != 0) {
				--number[it];
				ost = 0;
			}
			else {
				number[it] = last_val;
				ost = 1;
			}
			++it;
		}
		if (number[number.size() - 1] == 0)
			number.resize(number.size() - 1);
		return *this;
	}

	void swap(BigInteger& obj) {
		std::swap(is_neg, obj.is_neg);
		std::swap(number, obj.number);
	}

	int count_integer(int x) const {
		int count = 0;
		while (x != 0) {
			++count;
			x /= 10;
		}
		return count;
	}

	void erase_zeros() {
		int zeros = 0;
		for (int i = number.size() - 1; i >= 0; --i) {
			if (number[i] == 0)
				++zeros;
			else break;
		}
		if (zeros)
			number.resize(number.size() - zeros);
		if (number.size() == 0)
			number.push_back(0);
	}

	BigInteger(std::vector<int>& vals, int start, int end) {
		number = std::vector<int>(end - start + 1);
		int it = 0;

		for (int i = start; i <= end; ++i) {
			number[it] = vals[i];
			++it;
		}
	}

	void shift_right() {
		if (number.size() == 0) {
			number.push_back(0);
			return;
		}
		number.push_back(number[number.size() - 1]);
		for (size_t i = number.size() - 2; i > 0; --i)
			number[i] = number[i - 1];
		number[0] = 0;
	}

	void reverse() {
		std::reverse(number.begin(), number.end());
	}

	void check_zero() {
		if (number.size() == 1 && number[0] == 0)
			is_neg = false;
	}

	void combing() { //for multiply
		for (size_t i = 0; i < number.size(); ++i) {
			int ost;
			if (number[i] > last_val) {
				ost = number[i] / dec;
				number[i] %= dec;
				if (i == number.size() - 1)
					number.push_back(ost);
				else
					number[i + 1] += ost;
			}
		}
	}

	friend std::ostream& operator << (std::ostream& out, const BigInteger& num) {
		std::string a = num.toString();
		out << a;
		return out;
	}

	friend std::istream& operator >> (std::istream& in, BigInteger& num) {
		num.number.clear();
		num.is_neg = false;
		char digit;
		std::vector<char> n;
		in.get(digit);
		if (digit == '-') {
			num.is_neg = true;
		}
		else {
			n.push_back(digit - 48);
		}

		while (in.get(digit)) {
			if (digit == '-')
				num.is_neg = true;

			if (digit != '0' && digit != '5' && digit != '6' && digit != '9' &&
				digit != '1' && digit != '4' && digit != '7' && digit != '8' &&
				digit != '2' && digit != '3')
				break;
			n.push_back(digit - 48);
		}

		num.number.reserve((n.size() / base) + 10);
		for (int i = n.size() - 1; i >= 0; --i) {
			int ost = 0;
			int digit = 0;
			while (ost < base && i >= 0)
				digit += n[i] * pow(ost), ++ost, --i;
			++i;
			num.number.push_back(digit);
		}
		n.clear();
		num.check_zero();
		num.erase_zeros();
		return in;
	}

public:

	void check_sign() {
		if (number[0] == 0 && number.size() == 1)
			is_neg = false;
	}

	bool isEven() const {
		return (number[0] % 2 == 0);
	}

	void div2() {
		for (size_t i = 0; i < number.size(); ++i) {
			if (i != 0) number[i - 1] += 5000 * (number[i] % 2);
			number[i] /= 2;
		}
		if (number[number.size() - 1] == 0 && number.size() > 1) number.pop_back();
	}

	size_t get_digits() const {
		size_t size = count_integer(number[0]);
		size += (number.size() - 1) * 4;
		return size;
	}

	bool is_negative() const {
		return is_neg;
	}

	void inverse_sign() {
		is_neg = !is_neg;
	}

	BigInteger(int int_val) {
		if (int_val < 0) {
			is_neg = true;
			int_val = -int_val;
		}
		number.reserve(10);
		if (int_val == 0) number.push_back(0);
		while (int_val != 0) {
			number.push_back(int_val % dec);
			int_val /= dec;
		}
		check_zero();
	}

	BigInteger(const BigInteger& obj) {
		is_neg = obj.is_neg;
		number.resize(obj.number.size());
		std::copy(obj.number.begin(), obj.number.end(), number.begin());
	}

	BigInteger() {};

	std::string toString() const {
		std::string a;
		if (is_neg) a = "-";
		else a = "";
		if (number.size() == 0) return "";
		std::string digit = std::to_string(number[number.size() - 1]);
		std::string zeros;
		a += digit;
		for (int i = number.size() - 2; i >= 0; --i) {
			digit = std::to_string(number[i]);
			int zeros_num = base - count_integer(number[i]);
			zeros = std::string(zeros_num, '0');
			a.append(zeros);
			if (digit != "0")
				a.append(digit);
		}
		return a;
	}

	BigInteger& operator=(BigInteger obj) {
		swap(obj);
		return *this;
	}

	bool operator<= (const BigInteger& second_val) const {
		if (second_val.is_neg && is_neg) {
			if (number.size() < second_val.number.size()) return false;
			else if (number.size() > second_val.number.size()) return true;
			int iter_1 = number.size() - 1;
			while (iter_1 >= 0) {
				if (number[iter_1] != second_val.number[iter_1])
					return number[iter_1] > second_val.number[iter_1];
			}
			return true;
		}
		else if (second_val.is_neg) {
			return false;
		}
		else if (is_neg) {
			return true;
		}
		if (number.size() < second_val.number.size())
			return true;
		else if (number.size() > second_val.number.size())
			return false;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return number[iter_1] < second_val.number[iter_1];
			--iter_1;
		}
		return true;
	}

	void add_zeros(size_t count) {
		reverse();
		for (size_t i = 0; i < count; ++i)
			number.push_back(0);
		reverse();
	}

	bool operator >=(const BigInteger& second_val) const {
		if (second_val.is_neg && is_neg) {
			if (number.size() < second_val.number.size()) return true;
			else if (number.size() > second_val.number.size()) return false;
			int iter_1 = number.size() - 1;
			while (iter_1 >= 0) {
				if (number[iter_1] != second_val.number[iter_1])
					return number[iter_1] < second_val.number[iter_1];
			}
			return true;
		}
		else if (second_val.is_neg) {
			return true;
		}
		else if (is_neg) {
			return false;
		}
		if (number.size() < second_val.number.size()) return false;
		else if (number.size() > second_val.number.size()) return true;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return number[iter_1] > second_val.number[iter_1];
			--iter_1;
		}
		return true;
	}

	bool operator< (const BigInteger& second_val) const {
		return !(*this >= second_val);
	}

	bool operator> (const BigInteger& second_val) const {
		return !(*this <= second_val);
	}

	bool operator==(const BigInteger& second_val) const {
		if (second_val.is_neg != is_neg) return false;
		if (number.size() != second_val.number.size()) return false;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return false;
			--iter_1;
		}
		return true;
	}

	bool operator!=(const BigInteger& second_val) const {
		return !(*this == second_val);
	}

	BigInteger& operator+= (const BigInteger& second_val) {
		if (second_val.is_neg && !is_neg)
			return operate_minus(second_val);

		if (!second_val.is_neg && is_neg) {
			is_neg = !is_neg;
			operate_minus(second_val);
			is_neg = !is_neg;
			return *this;
		}

		return operate_plus(second_val);

	}

	BigInteger& operator-= (const BigInteger& second_val) {
		if (second_val.is_neg && !is_neg) {
			return operate_plus(second_val);
		}

		if (!second_val.is_neg && is_neg) {
			operate_plus(second_val);
			is_neg = true;
			return *this;
		}

		if (second_val.is_neg && is_neg) {
			is_neg = false;
			operate_minus(second_val);
			if (number.size() != 1 || number[0] != 0)
				is_neg = !is_neg;
			return *this;
		}

		return operate_minus(second_val);
	}

	BigInteger operator-() const {
		BigInteger new_obj = *this;
		if (new_obj.number.size() == 1 && number[0] == 0)
			return new_obj;
		new_obj.is_neg = !new_obj.is_neg;
		return new_obj;
	}

	BigInteger& operator++() {
		if (is_neg) return operate_double_minus();

		return operate_double_plus();

	}

	BigInteger operator++(int) {
		BigInteger copy = *this;
		++(*this);
		return copy;
	}

	BigInteger& operator--() {
		if (number.size() == 1 && number[0] == 0) {
			is_neg = true;
			number[0] = 1;
			return *this;
		}

		if (is_neg) {
			return operate_double_plus();
		}

		operate_double_minus();
	}

	BigInteger operator--(int) {
		BigInteger copy = *this;
		--(*this);
		return copy;
	}

	BigInteger& operator*=(const BigInteger& val) { //change sign
		std::vector<int> ans;
		ans.reserve(val.number.size() + number.size() + 10);
		int ost = 0;
		int value;
		size_t index;
		size_t our_size = number.size();
		size_t his_size = val.number.size();
		for (size_t i = 0; i < our_size; ++i) {
			for (size_t j = 0; j < his_size; ++j) {
				value = number[i] * val.number[j] + ost;
				ost = value / dec;
				value %= dec;
				index = i + j;
				if (index >= ans.size()) ans.push_back(0);
				ans[index] += value;
			}
			if (ost != 0) {
				if (i + his_size >= ans.size())
					ans.push_back(0);
				ans[i + his_size] += ost;
			}
			ost = 0;
		}
		number.clear();
		number = ans;
		erase_zeros();
		if (is_neg != val.is_neg) is_neg = true;
		else is_neg = false;
		combing();

		return *this;
	}

	BigInteger& operator/=(const BigInteger& num) {
		if (*this == num) { 
			*this = 1;
			return *this;
		}
		if (abs_less(num)) {
			number.clear();
			number.push_back(0);
			is_neg = false;
			return *this;
		}
		BigInteger result, current;
		result.is_neg = is_neg != num.is_neg;
		result.number.resize(number.size());
		for (int i = static_cast<int>(number.size()) - 1; i >= 0; --i) {
			current.shift_right();
			current.number[0] = number[i];
			current.erase_zeros();
			int x = 0, l = 0, r = dec;
			while (l <= r) {
				int m = (l + r) / 2;
				BigInteger t = num;
				t.is_neg = false;
				current.is_neg = false;
				t *= m;
				if (t <= current) {
					x = m;
					l = m + 1;
				}
				else r = m - 1;
			}

			result.number[i] = x;
			BigInteger mult = num;
			mult.is_neg = false;
			mult *= x;
			current -= mult;
		}
		result.is_neg = is_neg != num.is_neg;
		result.erase_zeros();
		swap(result);
		return *this;
	}

	BigInteger& operator %= (const BigInteger& num) {
		BigInteger del = *this;
		del /= num;
		del *= num;
		*this -= del;
		return *this;
	}

	explicit operator bool() {
		if ((number.size() == 1 && number[0] == 0) || number.size() == 0)
			return false;
		return true;
	}
};

BigInteger operator+(const BigInteger& first_val, const BigInteger& second_val) {
	BigInteger copy = first_val;
	copy += second_val;
	return copy;
}

BigInteger operator- (const BigInteger& first_val, const BigInteger& second_val) {
	BigInteger copy = first_val;
	copy -= second_val;
	return copy;
}

BigInteger operator* (const BigInteger& val_1, const BigInteger& val_2) {
	BigInteger copy = val_1;
	copy *= val_2;
	return copy;
}

BigInteger operator/ (const BigInteger& val_1, const BigInteger& val_2) {
	BigInteger copy = val_1;
	copy /= val_2;
	return copy;
}

BigInteger operator% (const BigInteger& val_1, const BigInteger& val_2) {
	BigInteger del = val_1 / val_2;
	del *= val_2;
	return val_1 - del;
}

BigInteger super_pow(size_t deg) {
	BigInteger ans = 1;
	for (size_t i = 0; i < deg; ++i) {
		ans *= 10;
	}
	return ans;
}

class Rational {

	BigInteger numerator;
	BigInteger denominator = 1;

	void check_sign() {
		numerator.check_sign();
	}
	BigInteger gcd(BigInteger A, BigInteger B) const {
		if (A.is_negative())
			A.inverse_sign();
		if (B.is_negative())
			B.inverse_sign();
		BigInteger k = 1;
		while (A && B)
		{
			while (A.isEven() && B.isEven())
			{
				A.div2();
				B.div2();
				k *= 2;
			}
			while (A.isEven()) A.div2();
			while (B.isEven()) B.div2();
			if (A >= B) A -= B; else B -= A;
		}
		return B * k;
	}

	void prettify() {
		check_sign();
		BigInteger g = gcd(numerator, denominator);
		numerator /= g;
		denominator /= g;
		if (denominator.is_negative()) {
			denominator.inverse_sign();
			numerator.inverse_sign();
		}
		if (numerator.is_negative()) {
			numerator.inverse_sign();
			if (numerator == 0)
				return;
			numerator.inverse_sign();
		}
		check_sign();
	}

public:

	std::string toString() const {
		std::string ans = numerator.toString();
		if (denominator != 1) {
			ans += '/';
			ans += denominator.toString();
		}
		return ans;

	}

	std::string asDecimal(size_t precision = 0) const {
		std::string ans;
		if (numerator < 0)
			ans += '-';
		BigInteger n1;
		if (numerator > 0)
			n1 = numerator;
		else
			n1 = -numerator;
		BigInteger whole = n1 / denominator;
		BigInteger after_comma = n1 % denominator;
		ans += whole.toString();
		if (precision > 0) {
			ans += ".";
			for (size_t i = 0; i < precision; ++i) {
				after_comma *= 10;
				BigInteger t = after_comma / denominator;
				ans += t.toString();
				after_comma %= denominator;
			}
		}
		return ans;
	}

	Rational() {

	}

	Rational(const BigInteger& number) {
		numerator = number;
	}

	Rational(int number) {
		numerator = number;
	}

	Rational(const Rational& number) {
		numerator = number.numerator;
		denominator = number.denominator;
	}

	Rational& operator += (const Rational& second_val) {
		numerator *= second_val.denominator;
		numerator += denominator * second_val.numerator;
		denominator *= second_val.denominator;
		prettify();
		return *this;
	}

	Rational& operator -=(const Rational& second_val) {
		numerator *= second_val.denominator;
		numerator -= denominator * second_val.numerator;
		denominator *= second_val.denominator;
		prettify();
		return *this;
	}

	Rational& operator *= (const Rational& second_val) {
		numerator *= second_val.numerator;
		denominator *= second_val.denominator;
		prettify();
		return *this;
	}

	Rational& operator /= (const Rational& second_val) {
		numerator *= second_val.denominator;
		denominator *= second_val.numerator;
		prettify();
		return *this;
	}

	Rational operator -() const {
		Rational copy = *this;
		if (copy != 0)
			copy.numerator.inverse_sign();
		return copy;
	}

	bool operator <= (const Rational& second_val) const {
		return numerator * second_val.denominator <= denominator * second_val.numerator;
	}

	bool operator >= (const Rational& second_val) const {
		return numerator * second_val.denominator >= denominator * second_val.numerator;
	}

	bool operator < (const Rational& second_val) const {
		return !(*this >= second_val);
	}

	bool operator > (const Rational& second_val) const {
		return !(*this <= second_val);
	}

	bool operator == (const Rational& second_val) const {
		return (numerator * second_val.denominator == denominator * second_val.numerator);
	}

	bool operator != (const Rational& second_val) const {
		return !(*this == second_val);
	}

	explicit operator double() const {
		return stod(asDecimal(16));
	}

	friend std::istream& operator >> (std::istream& in, Rational& obj) {
		long long a;
		in >> a;
		obj = a;
		return in;
	}

	friend std::ostream& operator << (std::ostream& out, const Rational& obj) {
		out << obj.toString();
		return out;
	}

};

Rational operator+(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy += second_val;
	return copy;
}

Rational operator-(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy -= second_val;
	return copy;
}

Rational operator*(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy *= second_val;
	return copy;
}

Rational operator/(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy /= second_val;
	return copy;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

template <int N>
class Finite {
protected:
	long long number = 0;

	long long opp(long long num) {
		int ost = N;
		int ros = 0;
		int hos = 1;
		int sost;
		int x;
		if (num == 0)
			return 1;
		while (num > 0)
		{
			sost = ost / num;
			x = num;
			num = ost % x;
			ost = x;
			x = hos;
			hos = ros - sost * x;
			ros = x;
		}
		ros %= N;
		if (ros < 0) ros = (ros + N) % N;
		return ros;
	}

public:
	Finite() = default;

	Finite(int num) {
		if (num >= 0) number = num % N;
		else number = (N + num % N) % N;
	}

	Finite(const Finite& num) : number(num.number) {}

	Finite<N>& operator=(const Finite<N>& num) {
		number = num.number;
		return *this;
	}

	Finite<N>& operator+=(const Finite<N>& num) {
		number += num.number;
		number %= N;
		return *this;
	}

	Finite<N>& operator-=(const Finite<N>& num) {
		number -= num.number;
		if (number < 0) number += N;
		return *this;
	}

	Finite<N>& operator*=(const Finite<N>& num) {
		number *= num.number;
		number %= N;
		return *this;
	}
	
	bool is_prime(size_t n) { // exponential
		bool result = true;
		for (int i = 2; i < n; ++i) {
			if (n % i == 0) {
				result = false;
				break;
			}
		}
		return result;
	}

	Finite<N>& operator/=(const Finite<N>& num) {
		if (!is_prime(N)) {
			throw std::invalid_argument("Facepalm");
		}
		long long oppos = opp(num.number);
		*this *= Finite<N>(oppos);
		return *this;
	}

	Finite<N>& operator++() {
		++number;
		if (number == N) number = 0;
		return *this;
	}

	Finite<N>& operator--() {
		--number;
		if (number == -1) number += N;
		return *this;
	}

	Finite<N> operator++(int) {
		Finite<N> copy = *this;
		++(*this);
		return copy;
	}

	Finite<N> operator--(int) {
		Finite<N> copy = *this;
		--(*this);
		return copy;
	}

	Finite<N> operator-() const {
		return (N - number) % N;
	}

	bool operator==(const Finite<N>& number) const {
		return this->number == number.number;
	}

	bool operator!=(const Finite<N>& number) const {
		return !(*this == number);
	}

	explicit operator int() const {
		return number;
	}

	friend std::ostream& operator <<(std::ostream& out, const Finite<N>& number) {
		out << number.number;
		return out;
	}

	~Finite() = default;

};

template <int N>
Finite<N> operator+(const Finite<N>& A, const Finite<N>& B) {
	Finite<N> copy = A;
	A += B;
	return copy;
}

template <int N>
Finite<N> operator-(const Finite<N>& A, const Finite<N>& B) {
	Finite<N> copy = A;
	copy -= B;
	return copy;
}

template <int N>
Finite<N> operator*(const Finite<N>& A, const Finite<N>& B) {
	Finite<N> copy = A;
	copy *= B;
	return copy;
}

template <int N>
Finite<N> operator/(const Finite<N>& A, const Finite<N>& B) {
	Finite<N> copy = A;
	copy /= B;
	return copy;
}

template <size_t M, size_t N, class Field = Rational>
class Matrix;

template <size_t N, class Field = Rational>
Field deter(Matrix< N, N, Field>);

template <size_t M, size_t N, class Field>
class Matrix {
	std::vector<std::vector<Field>> matrix = std::vector<std::vector<Field>>(M, std::vector<Field>(N));

public:
	Matrix() { 
		if (M != N) {
			throw std::invalid_argument("Good luck with creating non-square identity matrix");
		}
		for (size_t i = 0; i < M; ++i) {
			matrix[i][i] = 1;
		}
	}
	Matrix(const std::vector<std::vector<Field>>& to_copy) {
		matrix = to_copy;
	}

	Matrix(const std::vector<std::vector<int>>& to_copy) {
		for (size_t i = 0; i < M; ++i) {
			for (size_t j = 0; j < N; ++j) {
				matrix[i][j] = Field(to_copy[i][j]);
			}
		}
	}

	const std::vector<Field>& operator[] (size_t index) const {
		return matrix[index];
	}

	std::vector<Field>& operator[] (size_t index) {
		return matrix[index];
	}

	std::vector<Field> getRow(size_t index) const {
		return matrix[index];
	}

	std::vector<Field> getColumn(size_t index) const {
		std::vector<Field> col(M);
		for (size_t el = 0; el < M; ++el)
			col[el] = matrix[el][index];
		return col;
	}

	const Matrix<M, N, Field>& operator += (const Matrix<M, N, Field>& operand) {
		//for (size_t i = 0; i < M; ++i) {
		//	for (size_t j = 0; j < N; ++j) {
		//		std::cerr << matrix[i][j] << " ";
		//	}
		//	std::cerr << '\n';
		//}
		//std::cerr << '\n';
		for (size_t i = 0; i < M; ++i) {
			for (size_t j = 0; j < N; ++j) {
				matrix[i][j] += operand.matrix[i][j];
			}
		}
		return *this;
	}

	const Matrix<M, N, Field>& operator -= (const Matrix<M, N, Field>& operand) {
		for (size_t i = 0; i < M; ++i) {
			for (size_t j = 0; j < N; ++j) {
				matrix[i][j] -= operand.matrix[i][j];
			}
		}
		return *this;
	}

	const Matrix<M, N, Field>& operator *= (const Field& scalar) {
		for (size_t i = 0; i < M; ++i) {
			for (size_t j = 0; j < N; ++j) {
				matrix[i][j] *= scalar;
			}
		}
		return *this;
	}

	const Matrix<M, N, Field>& operator*= (const Matrix<N, N, Field>& op_2) { // too slow, fasten
		*this = *this * op_2;
		return *this;
	}

	Field det() const {
		return deter(*this);
	}

	template<size_t K, size_t S>
	bool operator==(const Matrix<K, S, Field>& op) const {
		if (K != M || S != N)
			return false;
		for (size_t i = 0; i < M; ++i) {
			for (size_t j = 0; j < N; ++j)
				if (matrix[i][j] != op.matrix[i][j]) return false;
		}
		return true;
	}

	template<size_t K, size_t S>
	bool operator!=(const Matrix<K, S, Field>& op1) const {

		return !(*this == op1);
	}

	Matrix<N, M, Field> transposed() const {

		std::vector<std::vector<Field>> transp(N, std::vector<Field>(M));
		for (size_t row = 0; row < M; ++row) {
			for (size_t col = 0; col < N; ++col) {
				transp[col][row] = matrix[row][col];
			}
		}
		return Matrix<N, M, Field>(transp);
	}

	size_t rank() const {
		if (M < N) return transposed().rank();
		Matrix<M, N, Field> copy = *this;
		size_t answer = N;
		for (size_t i = 0; i < N; ++i) {
			size_t cur_row = i;
			for (size_t j = i; j < M; ++j) {
				if (copy[j][i] != 0) {
					cur_row = j;
					break;
				}
			}
			if (copy[cur_row][i] == 0) --answer;
			else {
				std::swap(copy[cur_row], copy[i]);
				Field coef;
				for (size_t j = i + 1; j < M; ++j) {
					coef = copy[j][i] / copy[i][i];
					for (size_t k = i; k < N; ++k) {
						copy[j][k] -= coef * copy[i][k];
					}
				}
			}
		}
		return answer;
	}

	Field trace() const {

		Field trace = 0;
		for (size_t i = 0; i < M; ++i) {
			trace += matrix[i][i];
		}
		return trace;
	}

	Matrix<M, N, Field>& invert() {
		std::vector<std::vector<Field>> unity(M, std::vector<Field>(M));
		Field coef;
		for (size_t i = 0; i < M; ++i)
			unity[i][i] = 1;

		for (size_t i = 0; i < M; ++i) {
			size_t cur_row = i;
			for (size_t j = i; j < M; ++j) {
				if (matrix[j][i] != 0) {
					cur_row = j;
					break;
				}
			}
			if (matrix[cur_row][i] == 0) {
				matrix = unity;
				return *this;
			}
			std::swap(matrix[i], matrix[cur_row]);
			std::swap(unity[i], unity[cur_row]);
			for (size_t j = 0; j < M; ++j) {
				if (j != i) {
					coef = matrix[j][i] / matrix[i][i];
					for (size_t k = 0; k < M; ++k) {
						matrix[j][k] -= matrix[i][k] * coef;
						unity[j][k] -= unity[i][k] * coef;
					}
				}
			}
			coef = matrix[i][i];
			for (size_t j = 0; j < M; ++j) {
				matrix[i][j] /= coef;
				unity[i][j] /= coef;
			}
		}
		matrix = unity;
		return *this;
	}

	Matrix<M, N, Field> inverted() const {
		Matrix<M, N, Field> copy = *this;
		return copy.invert();
	}

};

template<size_t M, size_t N, class Field = Rational>
Matrix<M, N, Field> operator+(Matrix<M, N, Field> a, const Matrix<M, N, Field>& b) { 
	a += b;
	return a;
}

template<size_t M, size_t N, class Field = Rational>
Matrix<M, N, Field> operator-(Matrix<M, N, Field> a, const Matrix<M, N, Field>& b) { 
	a -= b;
	return a;
}

template<size_t M, size_t N, class Field = Rational>
Matrix<M, N, Field> operator*(const Field& a, Matrix<M, N, Field> b) { 
	b *= a;
	return b;
}

template <size_t N, class Field = Rational>
class SquareMatrix : public Matrix<N, N, Field> {
public:
	SquareMatrix() : Matrix<N, N, Field>() {}

	SquareMatrix(const std::vector<std::vector<Field>>& matrix) : Matrix<N, N, Field>(matrix) {}

	SquareMatrix(const std::vector<std::vector<int>>& matrix) : Matrix<N, N, Field>(matrix) {}

	SquareMatrix(const Matrix<N, N, Field>& matrix) : Matrix<N, N, Field>(matrix) {}

};

template <class Field = Rational>
Field row_to_col(const std::vector<Field>& row, const std::vector<Field>& col) { 
	size_t N = row.size();
	Field result = 0;
	for (size_t el = 0; el < N; ++el) {
		result += row[el] * col[el];
	}
	return result;
}

template <size_t N, size_t M, size_t K, class Field = Rational>
Matrix<M, K, Field> operator* (const Matrix<M, N, Field>& op_1, const Matrix<N, K, Field>& op_2) {
	std::vector<std::vector<Field>> result(M, std::vector<Field>(K));
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < K; ++j) {


			result[i][j] = row_to_col(op_1.getRow(i), op_2.getColumn(j));

		}
	}
	return result;
}

template <size_t N, class Field>
Field deter(Matrix < N, N, Field> matrix) {
	Field det = 1;
	Field coef = 1;
	size_t revs = 0;
	for (size_t cur_row = 0; cur_row < N - 1; ++cur_row) {
		size_t new_index = cur_row;
		for (size_t i = cur_row; i < N; ++i) {
			if (matrix[i][cur_row] != 0)
				new_index = i;
		}

		if (matrix[new_index][cur_row] == 0) { 
			return 0;
		}

		if (new_index != cur_row)
			std::swap(matrix[new_index], matrix[cur_row]), ++revs;
		for (size_t dec = cur_row + 1; dec < N; ++dec) {
			coef = -matrix[dec][cur_row] / matrix[cur_row][cur_row];
			for (size_t el = cur_row; el < N; ++el) {
				matrix[dec][el] += coef * matrix[cur_row][el];
			}
		}
	}

	for (size_t mid = 0; mid < N; ++mid)
		det *= matrix[mid][mid];

	if (revs % 2 != 0)
		det *= -1;

	return det;
}
