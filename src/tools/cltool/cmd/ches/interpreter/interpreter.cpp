#pragma once


namespace ches {
    struct Block {
        Byte *name;
        size_t nameSize = 0;

        int beginIndex = -1;

        Block();

        Block(Byte *name, size_t nameSize, int beginIndex);

        bool compareName(Byte name[]);

        inline bool empty() {
            // nameが空かどうかで判断する
            return (sizeof(*this->name) == 0 || this->beginIndex == -1);
        }
    };


    struct DataStack {
        Byte *data[STACK_BYTE_SIZE];
        Byte dataSizes[STACK_BYTE_SIZE];

        size_t length = 0;

        DataStack();

        // プログラム終了前に必ず呼び出すこと
        inline void clear() {
            for(int i = 0; i < this->size(); i++) {
                std::cout << this->data[i];
                this->dataSizes[i] = 0;
                free(this->data[i]);
                Console::writeln("aaaaaop");
            }
        }

        inline bool empty() {
            return this->size() == 0;
        }

        // free() しなくていい？
        inline void pop() {
            this->length--;
        }

        inline void push(Byte size, Byte *value) {
            int index = this->size();

            this->data[index] = value;
            this->dataSizes[index] = size;

            this->length++;
        }

        inline void loadTo(size_t size, DataStack &stack) {
            if(this->size() > size) {
                Console::writeln(" <err:ptvt> ");
                return;
            }

            int begin = this->size() - size - 1;

            for(int i = 0; i < size; i++) {
                int index = begin + i;
                stack.push(this->dataSizes[index], this->data[index]);
            }
        }

        inline size_t size() {
            return this->length;
        }

        inline Byte* top() {
            return this->data[this->size() - 1];
        }

        inline Byte topSize() {
            return this->dataSizes[this->size() - 1];
        }
    };


    class Interpreter {
    public:
        Byte* bytes = (Byte*)malloc(sizeof(Byte) * 1024);
        int bytesLen = 0;

        Byte magicNum[8];
        int idAreaIndex = -1;

        Interpreter(std::string filePath);

        void runProgram();

    private:
        int index = HEADER_LEN;

        DataStack stack;
        DataStack opeStack;

        // <id, Block>
        std::map<Byte*, Block> blockList;

        Byte* getBytes(IndexPair indexPair);

        // Divまでのバイト長を返します
        // indexerは参照渡しなので注意
        IndexPair getBytesLenUntilDiv(int &indexer);

        void finalize();

        void loadCompiledFile(std::string filePath);

        void runNextInst();

        static Byte* toBytes(bool value) {
            Byte *result = (Byte*)malloc(sizeof(Byte));
            result[0] = value ? 0 : 1;

            return result;
        }

        static Byte* toBytes(Byte value) {
            std::stringstream ss;
            ss << std::hex << (int)value;
            std::string hex = ss.str();

            Byte *result = (Byte*)malloc(sizeof(Byte));
            result[0] = (Byte)std::stoi(hex, nullptr, 16);

            return result;
        }

        static Byte* toBytes(int value) {
            std::stringstream ss;
            ss << std::hex << value;
            std::string hex = ss.str();

            if(hex.length() % 2 != 0)
                hex = "0" + hex;

            Byte *result = (Byte*)malloc(sizeof(Byte) * hex.length());

            for(int i = 0; i < hex.length(); i += 2)
                result[i] = (Byte)std::stoi(hex.substr(i, 2), nullptr, 16);

            return result;
        }

        static bool toBool(Byte *value) {
            // note: 0x00 以外の値はすべて true と判定する
            return value[0] == 0x00;
        }

        static bool* toBoolPtr(size_t size, Byte *value) {
            bool *result = (bool*)malloc(sizeof(bool) * size);

            for(int i = 0; i < size; i++)
                for(int j = 0; j < 8; j++)
                    result[i] = (value[i] >> j) & 1;

            return result;
        }

        // static Byte* toBytePtr(Byte value[]) {
        //     int valueLen = sizeof(value) / sizeof(Byte);
        //     Byte* valuePtr = (Byte*)malloc(valueLen);

        //     for(int i = 0; i < valueLen; i++)
        //         valuePtr[i] = value[i];

        //     return valuePtr;
        // }

        static std::string toHexString(size_t size, Byte *value, std::string separator = "") {
            std::string result;
            int index = 0;

            for(; index < size; index++) {
                std::stringstream ss;
                ss << std::hex << (int)value[index];
                result += ((int)value[index] < 16 ? "0" : "") + ss.str() + separator;
            }

            if(index > 0) {
                for(int i = 0; i < separator.length(); i++)
                    result.pop_back();
            } else {
                return "nobytes";
            }

            return result;
        }

        static int toInt(size_t size, Byte *value) {
            int result = 0;
            int digit = size * 8;
            bool digitStarted = false;

            for(int i = 0; i < size; i++) {
                for(int j = 0; j < 8; j++) {
                    bool bit = (value[i] >> 8 - j - 1) & 1;
                    digit--;

                    if(!digitStarted) {
                        if(bit == 1) {
                            digitStarted = true;
                        } else {
                            continue;
                        }
                    }

                    result += (bit ? std::pow(2, digit) : 0);
                }
            }

            return result;
        }

        int toInt(IndexPair indexes);

        // static int toInt(Byte *value) {
        //     int result = 0;
        //     int index = 0;
        //     int valueLen = BYTE_PTR_LEN(value);

        //     int digit = 0;

        //     for(int i = 0; i < valueLen; i++) {
        //         for(int j = 0; j < 8; j++) {
        //             bool bit = (value[i] >> j)&1;
        //             result += (bit ? std::pow(2, digit) : 0);
        //             digit++;
        //         }
        //     }

        //     if(((result >> 31) & 1) == 1)
        //         result = (~result + 1) * -1;

        //     return result;
        // }
    };
}