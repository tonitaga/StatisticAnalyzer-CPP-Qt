/** Statistic analyzer header file
 * 
 * Copyright (C) Gubaydullin Nurislam, School 21. All Rights Reserved
 * 
 * If the code of this project has helped you in any way,
 * please thank me with a cup of beer.
 * 
 * gmail:    gubaydullin.nurislam@gmail.com
 * telegram: @tonitaga
 */

#ifndef STATISTIC_ANALYZER_MODEL_DATAREADER_DATA_READER_H_
#define STATISTIC_ANALYZER_MODEL_DATAREADER_DATA_READER_H_

#include <vector>
#include <string>
#include <fstream>
#include <iterator>

namespace s21 {
    /**
     * @class NormalDistributionStatisticReader class считывает данные
     * типа int, double, float, с файла и возвращает считанные данные в виде std::vector
     *
     * @methods
     * a) read(const std::string &) -> static function,\n
     *      input argument is reading file path,\n
     *      output is std::vector<double> with read data,\n
     *      vector will be empty if file doesn't opened successfully
     *
     * @how_to_use
     * a) s21::NormalDistributionStatisticReader::read(path)\n
     * b) s21::NormalDistributionStatisticReader reader;\n
     *   reader.read(path);
     *
     * @file_structure
     * a) like this:\n 1 2 3 4 5\n
     * b) like this:\n
     *  1\n
     *  2\n
     *  3\n
     */
    class NormalDistributionStatisticReader final {
    public:
        static std::vector<double> read(const std::string &path) {
            std::ifstream fin(path);
            if (!fin.is_open())
                return {};
            std::vector<double> statistic{std::istream_iterator<double>(fin), std::istream_iterator<double>()};
            fin.close();
            return statistic;
        }
    };
}

#endif // !STATISTIC_ANALYZER_MODEL_DATAREADER_DATA_READER_H_
