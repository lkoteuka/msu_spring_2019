#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <string.h>

std::string input = "input.bin";

//получаем количество чисел в файле
uint64_t file_size(std::string &filename)
{
    FILE *input_file = fopen(filename.c_str(), "rb");
    fseek(input_file, 0, SEEK_END);
    uint64_t size = ftell(input_file) / sizeof(uint64_t);
    fclose(input_file);
    return size;
}

//merge двух файлов
void my_mergefile(std::string file1, std::string file2, std::string out_file) {
    FILE* out = fopen(out_file.c_str(), "wb");
    FILE* left = fopen(file1.c_str(), "rb");
    FILE* right = fopen(file2.c_str(), "rb");

    //std::unique_ptr<FILE, int(*)(FILE*)> out(fopen(out_file.c_str(), "wb"), &fclose);
    
    uint64_t left_size = file_size(file1);
    uint64_t right_size = file_size(file2);
    uint64_t tmp1, tmp2;

    fread(&tmp1, sizeof(uint64_t), 1, left);
    fread(&tmp2, sizeof(uint64_t), 1, right);

    //сортируем числа первого и второго файлов и сохраняем в out
    while(left_size != 0 || right_size != 0) {
        if (right_size == 0){
            left_size--;
            fwrite(&tmp1, sizeof(uint64_t), 1, out);
            if(left_size) fread(&tmp1, sizeof(uint64_t), 1, left);
        } else if (left_size == 0){
            right_size--;
            fwrite(&tmp2, sizeof(uint64_t), 1, out);
            if(right_size) fread(&tmp2, sizeof(uint64_t), 1, right);
        } else if(tmp1 <= tmp2) {
            left_size--;
            fwrite(&tmp1, sizeof(uint64_t), 1, out);
            if(left_size) fread(&tmp1, sizeof(uint64_t), 1, left);
        } else if(tmp1 > tmp2){
            right_size--;
            fwrite(&tmp2, sizeof(uint64_t), 1, out);
            if(right_size) fread(&tmp2, sizeof(uint64_t), 1, right);
        }
    }
    fclose(out);
    fclose(left);
    fclose(right);
}

std::string my_mergesort(uint step, uint64_t left, uint64_t right) {
    //пока не одно число, "разделяем" файл затем соединяем в один файл с сортировкой
    if (left != right){
        uint64_t mid = (left + right) / 2;
        std::string file1 = my_mergesort(step + 1, left, mid);
        std::string file2 = my_mergesort(step + 1, mid + 1, right);
        std::string out_file = std::string(std::to_string(step) + "_" + std::to_string(left) + '_' + std::to_string(right));
        my_mergefile(file1, file2, out_file);
        return out_file;
    } else {
        std::string out_file = std::string(std::to_string(step) + "_" + std::to_string(left) + '_' + std::to_string(right));
        FILE* out = fopen(out_file.c_str(), "wb");
        FILE* in = fopen(input.c_str(), "rb");
        uint64_t tmp;
        fseek(in, sizeof(uint64_t) * left, SEEK_SET);
        fread(&tmp, sizeof(uint64_t), 1, in);
        fwrite(&tmp, sizeof(uint64_t), 1, out);
        fclose(out);
        fclose(in);
        return out_file;
    }
}

int main(int argc, char **argv)
{
    uint64_t size = file_size(input);
    std::string file1 = std::string("0_0_" + std::to_string((size - 1) / 2));
    std::string file2 = std::string("0_" + std::to_string((size - 1) / 2 + 1) + '_' + std::to_string(size - 1));

    //потоки сортируют разные части файла
    std::thread thread1(my_mergesort, 0, 0, (size - 1) / 2);
    std::thread thread2(my_mergesort, 0, (size - 1) / 2 + 1, size - 1);
    thread1.join();
    thread2.join();

    //соединяем отсортированные части в единый файл, когда потоки завершились
    my_mergefile(file1, file2, "output.bin");
    return 0;
}