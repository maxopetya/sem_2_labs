//
// Created by maxop on 12.03.2026.
//

#include<iostream>
#include<algorithm>

class String {
    private:
        char *str;
        size_t length=0;

    public:
    String() : str(nullptr), length(0) {}

    //констуктор копирования
    String (const String& rhs) : str(nullptr), length(rhs.length) {
        if (rhs.str) {
            str = new char[length];
            std::copy(rhs.str, rhs.str + length, str);
        }
    }

    //пользовательский конструктор
    String (const char* data) : str(nullptr), length(0) {
        if (data) {
            while (*(data+length) != '\0') {
                length++;
            }
            str = new char[length];
            std::copy(data, data + length, str);
        }
    }
    //конструктор перемещения
    String (String&& rhs) : str(rhs.str), length(rhs.length) {
        rhs.length = 0;
        rhs.str = nullptr;
    }

    //деструктор
    ~String() {
        delete[] str;
    }

    //присваивание
    String& operator= (const String& rhs) {
        if (&rhs!=this) {
            delete[] str;
            length = rhs.length;
            if (rhs.str) {
                str = new char[length];
                std::copy(rhs.str, rhs.str + length, str);
            } else {
                str = nullptr;
            }
        }
        return *this;
    }

    //присваивание со сложением (конкатенацией)
    String& operator+=(const String& rhs) {
        if (!rhs.str || rhs.length == 0) return *this;
        if (!str) {
            *this = rhs;
            return *this;
        }
        char* temp = new char[length+rhs.length];
        std::copy(str, str + length, temp);
        std::copy(rhs.str, rhs.str + rhs.length, temp+length);
        length += rhs.length;
        delete[] str;
        str=temp;
        return *this;
    }

    //присваивание с повторением
    String& operator*=(unsigned int m) {
        if (!str || m == 0 || m == 1) return *this;
        char* temp = new char[length*m];
        for (int i=0; i<m; i++) {
            std::copy(str, str + length, temp+i*length);
        }
        length *= m;
        delete[] str;
        str=temp;
        return *this;
    }

    //сравнение (равенство)
    bool operator==(const String& rhs) const {
        if (length != rhs.length) return false;
        if (!str && !rhs.str) return true;
        if (!str || !rhs.str) return false;
        for (int i=0; i<length; i++) {
            if (*(str+i) != *(rhs.str+i)) return false;
        }
        return true;
    }

    //сравнение (меньше)
    bool operator<(const String& rhs) const {
        if (!str && !rhs.str) return false;
        if (!str) return true;
        if (!rhs.str) return false;
        for (int i=0; i<std::min(length,rhs.length); i++) {
            if (*(str+i) < *(rhs.str+i)) return true;
            if (*(str+i) > *(rhs.str+i)) return false;
        }
        if (length < rhs.length) return true;
        return false;
    }

    //поиск подстроки
    long long Find (const String& substr) const {
        if (!str || !substr.str || length<substr.length) return -1;
        bool found;
        for (int i=0; i<=length-substr.length; i++) {
            found=true;
            for (int j=0; j<substr.length; j++) {
                if (*(str+i+j) != *(substr.str+j)) {found=false;break;}
            }
            if (found) return i;
        }
        return -1;
    }

    //замена
    void Replace(char oldSymbol, char newSymbol) {
        if (!str) return;
        for (int i=0; i<length; i++) {
            if (*(str+i) == oldSymbol) {*(str+i) = newSymbol;}
        }
    }


    size_t Size() const {
        return length;
    }

    bool Empty() const {
        return length==0;
    }

    char operator[](size_t index) const {
        return (str && index < length) ? str[index] : '\0';
    }

    char& operator[](size_t index) {
        static char dummy = '\0';
        return (str && index < length) ? str[index] : dummy;
    }

    //обрезка строки
    void RTrim(char symbol) {
        if (!str) return;
        while (length > 0 && str[length-1] == symbol) {
            length--;
        }
        if (length == 0) {
            delete[] str;
            str = nullptr;
            return;
        }
        char* temp = new char[length];
        std::copy(str, str + length, temp);
        delete[] str;
        str=temp;
    }

    void LTrim(char symbol) {
        if (!str) return;
        size_t shift=0;
        while (shift < length && str[shift] == symbol) {
            shift++;
        }
        if (shift == length) {
            delete[] str;
            str = nullptr;
            length = 0;
            return;
        }
        length-=shift;
        char* temp = new char[length];
        std::copy(str + shift, str + shift + length, temp);
        delete[] str;
        str=temp;
    }

    //обмен
    void swap(String& oth) {
        char* temp_str = str;
        int temp_length = length;
        str = oth.str;
        length = oth.length;
        oth.str = temp_str;
        oth.length = temp_length;
    }

    //дружественный вывод
    friend std::ostream& operator<<(std::ostream& os, const String& Str) {
        if (Str.str) {
            for (int i=0; i<Str.length; i++) {
                os << *(Str.str+i);
            }
        }
        return os;
    }
};

//конкатенация
String operator+(const String& a, const String& b) {
    String result(a);
    result += b;
    return result;
}

//повторение
String operator*(const String& a, unsigned int b) {
    String result(a);
    result *= b;
    return result;
}

//неравно
bool operator!=(const String& a, const String& b) {
    return !(a==b);
}

//больше
bool operator>(const String& a, const String& b){
    if (a==b) return false;
    return !(a<b);
}

int main() {
    std::cout << "String Class Demonstration\n\n";

    std::cout << "1. Constructors:\n";
    String s1;
    String s2 = "Hello";
    String s3(nullptr);
    String s4 = s2;
    String s5 = std::move(String("Temporary"));

    std::cout << "s1 (default): \"" << s1 << "\", size: " << s1.Size()
              << ", empty: " << (s1.Empty() ? "true" : "false") << "\n";
    std::cout << "s2 (from C-string): \"" << s2 << "\", size: " << s2.Size() << "\n";
    std::cout << "s3 (from nullptr): \"" << s3 << "\", size: " << s3.Size() << "\n";
    std::cout << "s4 (copy of s2): \"" << s4 << "\"\n";
    std::cout << "s5 (move from temporary): \"" << s5 << "\"\n\n";

    std::cout << "2. Assignment:\n";
    String s6;
    s6 = s2;
    std::cout << "s6 = s2: \"" << s6 << "\"\n";
    s6 = String("New Value");
    std::cout << "s6 = temporary: \"" << s6 << "\"\n";
    s6 = s6;
    std::cout << "s6 = s6 (self-assignment): \"" << s6 << "\"\n\n";

    std::cout << "3. Concatenation and repetition:\n";
    String s7 = "Hello";
    String s8 = " World";
    s7 += s8;
    std::cout << "\"Hello\" += \" World\": \"" << s7 << "\"\n";

    String s9 = String("C++") + String(" is ") + String("awesome");
    std::cout << "\"C++\" + \" is \" + \"awesome\": \"" << s9 << "\"\n";

    String s10 = "Ha";
    s10 *= 3;
    std::cout << "\"Ha\" *= 3: \"" << s10 << "\"\n";

    String s11 = String("Ab") * 4;
    std::cout << "\"Ab\" * 4: \"" << s11 << "\"\n\n";

    std::cout << "4. Comparison:\n";
    String cmp1 = "apple";
    String cmp2 = "banana";
    String cmp3 = "apple";
    String cmp4 = "app";

    std::cout << "cmp1 = \"" << cmp1 << "\", cmp2 = \"" << cmp2 << "\"\n";
    std::cout << "cmp1 == cmp2: " << (cmp1 == cmp2 ? "true" : "false") << "\n";
    std::cout << "cmp1 == cmp3: " << (cmp1 == cmp3 ? "true" : "false") << "\n";
    std::cout << "cmp1 != cmp2: " << (cmp1 != cmp2 ? "true" : "false") << "\n";
    std::cout << "cmp1 < cmp2: " << (cmp1 < cmp2 ? "true" : "false") << "\n";
    std::cout << "cmp1 < cmp4: " << (cmp1 < cmp4 ? "true" : "false") << "\n";
    std::cout << "cmp4 < cmp1: " << (cmp4 < cmp1 ? "true" : "false") << "\n\n";

    std::cout << "5. Index access:\n";
    String s13 = "Programming";
    std::cout << "s13 = \"" << s13 << "\"\n";
    std::cout << "s13[0] = '" << s13[0] << "', s13[3] = '" << s13[3] << "'\n";
    s13[0] = 'X';
    s13[5] = 'Y';
    std::cout << "After s13[0] = 'X', s13[5] = 'Y': \"" << s13 << "\"\n\n";

    std::cout << "6. Find and Replace:\n";
    String s14 = "Hello world, hello universe";
    String substr = "hello";
    std::cout << "s14 = \"" << s14 << "\"\n";
    std::cout << "Find \"" << substr << "\" at position: " << s14.Find(substr) << "\n";
    s14.Replace('l', 'L');
    std::cout << "After Replace('l', 'L'): \"" << s14 << "\"\n\n";

    std::cout << "7. Trim operations:\n";
    String s15 = "___Hello___";
    std::cout << "Original: \"" << s15 << "\"\n";
    s15.LTrim('_');
    std::cout << "After LTrim('_'): \"" << s15 << "\"\n";
    s15.RTrim('_');
    std::cout << "After RTrim('_'): \"" << s15 << "\"\n\n";

    std::cout << "8. Size and empty:\n";
    std::cout << "s2 size: " << s2.Size() << ", empty: " << (s2.Empty() ? "true" : "false") << "\n";
    std::cout << "s1 size: " << s1.Size() << ", empty: " << (s1.Empty() ? "true" : "false") << "\n\n";

    std::cout << "9. Swap:\n";
    String a = "First";
    String b = "Second";
    std::cout << "Before swap: a = \"" << a << "\", b = \"" << b << "\"\n";

    using std::swap;
    swap(a, b);

    std::cout << "After swap: a = \"" << a << "\", b = \"" << b << "\"\n\n";

    std::cout << "10. Edge cases:\n";
    String empty;
    String single = "A";
    std::cout << "Empty string find 'A': " << empty.Find(single) << "\n";
    empty += empty;
    std::cout << "Empty += empty: \"" << empty << "\"\n";
    empty *= 5;
    std::cout << "Empty *= 5: \"" << empty << "\"\n";
    String space = "   ";
    space.RTrim(' ');
    std::cout << "Trim spaces from \"   \": \"" << space << "\"\n\n";


    return 0;
}