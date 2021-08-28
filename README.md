# Parallel C++ linalg library
A group project to create a basic linear algebra library in C++ that takes advantage of parallel programming.

## Objectives of the project
*TODO*

## How to compile
*TODO*, but probably using make.

## Authors
The project is done by me, [InfinityZxc](https://github.com/InfinityZxc) and [0masiza0](https://github.com/0masiza0)

## License
The library is licensed under the MIT license.

## documentation
"Matrix"

{
конструкторы:

(size_t rows, size_t columns)
создает матрицу размера rows * columns

(const std::vector<T>& vec)
создает матрицу размера 1 * (vec.size()) с элементами vec

(std::vector<T>&& vec)
аналогично предыдущему, но захватывает память vec

(const std::vector<std::vector<T>>& matrix)
создает матрицу размера matrix.size() * matrix.front().size()

(std::vector<std::vector<T>>&& matrix)
аналогично предыдущему, но захватывает память matrix

(const Matrix<T>& other)
создает копию other

(Matrix<T>&& other)
создает копию, захватывая память other

для всех конструкторов тип Т должен соответствовать требованиям для создания vector<T>

}

{

operator=

Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
создает копию other

Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other)
создает копию, захватывая память other

}

{

operator==

bool Matrix<T>::operator==(const Matrix<T>& other) const
сравнивает размер и элементы *this и other
возвращает true если всё совпадает, false иначе

}

{

const std::vector<T>& Matrix<T>::operator[](size_t index) const
возвращает вектор - строку с индексом index без возможности изменить элементы

std::vector<T>& Matrix<T>::operator[](size_t index)
возвращает ссылку на вектор - строку с индексом index с возможностью изменить элементы

}

{

operator!=
(const Matrix<T>& other) const
вызывает !(*this == other)

}

{

operator+
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const
возвращает матрицу поэлементных сумм *this и other, требует от Т определенного оператора +

operator-
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const
возвращает матрицу поэлементных разностей *this и other, требует от Т определенного оператора -

operator*
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const
возвращает матрицу поэлементных произведений *this и other, требует от Т определенного оператора *

Matrix<T> Matrix<T>::operator*(const T& other_const) const
возвращает матрицу поэлементно умноженную на other_const, требует от Т определенного оператора *

operator+=(const Matrix<T>& other)

operator-=(const Matrix<T>& other)

operator*=(const Matrix<T>& other)

operator*=(const T& other_const)
аналоги возвращающие измененный *this

}

{

Matrix<T>::shape() const
возвращает кол-во строк и столбцов матрицы в виде
std::pair<size_t, size_t>

}

Matrix<T>::basic_binary_op_(const Matrix<T>& other,
char func_type) const

{

Matrix<T>::print(std::ostream& out, char column_separator,
char row_separator) const
выводит матрицу в поток out, разделяя элементы в одной строке символом column_separator, строки - символом row_separator.

operator<<(std::ostream& out, const Matrix<T>& mat) {
вызывает mat.print(out, '\t', '\n')

}

{

Matrix<T> Matrix<T>::eye(size_t size)
возвращает квадратную матрицу размера size*size со значением Т(1) на диагонали
требует от Т определенного конструктора Т(1)

}

DLC:

{

Matrix<T> Dot(const Matrix<T>& l, const Matrix<T>& r)
возвращает матричное произведение l на r

требует от Т определенных операторов + и *

Matrix<T> Dot_single_thread(const Matrix<T>& l, const Matrix<T>& r)
аналогично предыдущему, но выполняет все последовательно

}

{

Matrix<T> Transpose(const Matrix<T>& matrix)
возвращает транспонированную матрицу matrix

Matrix<T> Transpose_single_thread(const Matrix<T>& matrix)
аналогично предыдущему, но выполняет все последовательно

}

{

T det(const Matrix<T>& a)
возвращает определитель матрицы а
если матрица не квадратная, то кидает исключение "Matrix should be square\n"

требует от Т определенных операций +,-,*,/, конструктора Т(0) создающего нейтральный для операции + элемент.

}

{

Equation
выполняет операции со строками

std::vector<T> Equation::compose_rows(const std::vector<T>& row1,const std::vector<T>& row2,const T& coef)
возвращает сумму строк row1 и (row2, умноженной на скаляр coef)
требует от Т определенных операций +,*
может кинуть "Vectors have different size"

std::vector<T> Equation::multiply_by_scalar(const std::vector<T>& row,const T& scalar)
возвращает строку row, умноженную на скаляр scalar
требует от Т определенных операций +,*, конструктора Т(0) создающего нейтральный для операции + элемент.
может кинуть "Cannot multiply by zero"

void Equation::swap_rows(std::vector<T>& row1, std::vector<T>& row2)
меняет местами row1 и row2

}

{

Matrix<T> Solve(const Matrix<T>& a, const Matrix<T>& b)
решает слу с единственным решением, где а - матрица системы, b - столбец свободных членов

если матрица а не квадратная, то кидает исключение "Matrix A should be square\n"
если высота b не совпадает с высотой a, то кидает исключение "Matrixes should be conformable\n"

требует от Т определенных операций +,-,*,/, конструктора Т() создающего нейтральный для операции + элемент, конструктора Т(1) создающего нейтральный для операции * элемент.

}

{

Matrix<T> Inv(const Matrix<T>& a)
возвращает обратную матрицу
требует условия для Solve

}

{

std::pair<Matrix<T>, Matrix<T>> FindQR(const Matrix<T>& a)
вычисляет QR разложение матрицы а
если матрица а не квадратная, то кидает исключение "Matrix \"a\" should be square\n"
требует от Т определенных операций +,-,*,/, конструктора Т() создающего нейтральный для операции + элемент, конструктора Т(1) создающего нейтральный для операции * элемент.

bool IsUpTriangular(const Matrix<T>& a, const T& delta)
проверяет элементы а ниже диагонали из левого верхнего угла, если все меньше delta, то возвращает true
требует от Т определенной операции -, операторов <,>, конструктора Т() создающего нейтральный для операции + элемент.

std::vector<std::vector<T>> SolveMultipleSolutions(Matrix<T>&& a,const T& delta)
решает слу с несколькими решениями, считая элементы меньше delta по модулю равными нулю.
если матрица а не квадратная, то кидает исключение "Matrix \"a\" should be square\n"
требует от Т определенной операции +,-,*,/, операторов <,>, конструктора Т(1) создающего нейтральный для операции * элемент.

std::pair<std::vector<T>, Matrix<T>> Eig(const Matrix<T>& a, const T& delta)

std::pair<Matrix<T>, Matrix<T>> FindQRSingular(const Matrix<T>& a)
аналогична функции выше, но выполняет все последовательно

bool IsUpTriangularSingular(const Matrix<T>& a, const T& delta)
аналогична функции выше, но выполняет все последовательно

std::vector<std::vector<T>> SolveMultipleSolutionsSingular(Matrix<T>&& a,const T& delta)
аналогична функции выше, но выполняет все последовательно

std::pair<std::vector<T>, Matrix<T>> EigSingular(const Matrix<T>& a,const T& delta)
аналогична функции выше, но выполняет все последовательно

}

{

ThreadPool::ThreadPool(size_t threads)
менеджер нитей, использующий одновременно не более чем threads штук

void ThreadPool::enqueue_task(func f)
добавляет f в очередь на выполнение

ThreadPool::~ThreadPool()
ждет завершения всех функций переданных через enqueue_task функций

}

{

T Trace(const Matrix<T>& a)
возвращает след матрицы a
если матрица а не квадратная, то кидает исключение "Matrix should be square\n"
требует от Т определенной операции +, конструктора Т() создающего нейтральный для операции + элемент.

}

{

Matrix<T> MatrixPower(const Matrix<T>& a, size_t n)
возводит матрицу а в степень n
если матрица а не квадратная, то кидает исключение "Matrix should be square\n"
использует Dot и eye

}

{

Matrix<T> Split_column(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step)
возвращает матрицу из столбцов от from включительно до to не включительно с шагом step
если невозможно достичь to из from с помощью step, то кидает "Impossible arguments\n"

Matrix<T> Split_column_single_thread(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step)
аналогична функции выше, но выполняет все последовательно

Matrix<T> Split_row(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step)
возвращает матрицу из строк от from включительно до to не включительно с шагом step
если невозможно достичь to из from с помощью step, то кидает "Impossible arguments\n"

Matrix<T> Split_row_single_thread(const Matrix<T>& mat, const int64_t from, const int64_t to, const int64_t step)
аналогична функции выше, но выполняет все последовательно

}
