#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

using std::cin;
using std::cout;
using std::istringstream;
using std::string;

// Matrix entry type. We really only need a bit per matrix entry; char is the
// smallest native type.
typedef char matrix_entry_t;

class matrix {
    private:
        int size_;
        // We store the matrix as one linear array instead of as a
        // two-dimensional array.  This guarantees that the matrix is stored in
        // one contiguous chunk of memory, giving better cache properties and
        // resulting in better performance.  The matrix is stored as:
        // row1 ... rowN, so the ij-th entry is stored at location i*size_+j.
        matrix_entry_t* matrix_;
    public:
        matrix(int size): size_(size) {
            matrix_ = (matrix_entry_t *) calloc(size_ * size_,
                    sizeof(matrix_entry_t));
            if (matrix_ == NULL) {
                std::cerr << "Out of memory.\n";
                exit(1);
            }
        }

        matrix(const matrix& A) {
            size_ = A.size_;
            matrix_ = (matrix_entry_t *) calloc(size_ * size_,
                    sizeof(matrix_entry_t));
            for (int i = 0; i < size_ * size_; ++i) {
                matrix_[i] = A.matrix_[i];
            }
        }

        matrix operator=(const matrix& A) {
            for (int i = 0; i < size_ * size_; ++i) {
                matrix_[i] = A.matrix_[i];
            }
            return *this;
        }

        ~matrix() {
            free(matrix_);
        }

        inline matrix_entry_t get(int i, int j) const {
            return matrix_[i * size_ + j];
        }

        inline matrix_entry_t set(int i, int j, matrix_entry_t value){
            return (matrix_[i * size_ + j] = value);
        }

        matrix operator+(const matrix &other) {
            matrix result(size_);
            for (int i = 0; i < size_ * size_; ++i) {
                result.matrix_[i] = matrix_[i] | other.matrix_[i];
            }
            return result;
        }

    private:
        void multiply(const matrix &other, matrix* result) {
            for (int i = 0; i < size_; ++i) {
                for (int k = 0; k < size_; ++k) {
                    for (int j = 0; j < size_; ++j) {
                        result->set(i, j,
                                result->get(i, j) |
                                (get(i, k) & other.get(k, j)));
                    }
                }
            }
        }

    public:
        matrix operator*(const matrix &other) {
            matrix result(size_);
            multiply(other, &result);
            return result;
        }

        /* Matrix exponentiation by repeated squaring. Running time is O(T log
         * N), where T is the time for matrix multiplication. Naive
         * exponentiation is O(T N).
         */
        matrix power(int n) {
            matrix result(size_), arg = *this;
            matrix temp(size_);
            for (int i = 0; i < size_; ++i) {
                result.set(i, i, 1);
            }
     
            while (n > 0)
            {
                if (n & 1) {
                    result.multiply(arg, &temp);
                    result = temp;
                }
                if (n > 1) {
                    arg.multiply(arg, &temp);
                    arg = temp;
                }
                n >>= 1;
            }
            return result;
        }
};


int main() {
    // Number of vertices
    int n;
    cin >> n;
    cin.ignore(1, '\n');

    // Adjacency matrix
    matrix adjacency(n);

    // Read in adjacency matrix
    for (int i = 0; i < n; ++i) {
        int vertex;
        string line;
        getline(cin, line);
        istringstream iss(line);
        iss.peek();
        while (!iss.eof()) {
            iss >> vertex;
            adjacency.set(i, vertex, 1);
        }
    }

    // Adjacency matrix plus identity
    matrix full_adjacency(n);
    for (int i = 0; i < n; ++i) {
        full_adjacency.set(i, i, 1);
    }
    full_adjacency = full_adjacency + adjacency;
    matrix final_result = adjacency * adjacency * full_adjacency.power(n - 1);

    // Remove all edges with nonzero entries in final_result,
    // and print the answer
    for (int i = 0; i < n; ++i) {
        int first = 1;
        for (int j = 0; j < n; ++j) {
            adjacency.set(i, j, adjacency.get(i, j) & ~final_result.get(i, j));
            if (adjacency.get(i, j)) {
                if (!first) {
                    cout << " ";
                }
                cout << j;
                first = 0;
            }
        }
        cout << "\n";
    }
    return 0;
}
