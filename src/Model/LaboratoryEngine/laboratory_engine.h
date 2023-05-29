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

#ifndef STATISTIC_ANALYZER_MODEL_LABORATORYENGINE_LABORATORY_ENGINE_H_
#define STATISTIC_ANALYZER_MODEL_LABORATORYENGINE_LABORATORY_ENGINE_H_

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>

namespace s21 {
    /**
     * @class LaboratoryStatisticEngine\n
     * Может конструироваться от данных считанных с файла, либо используя метод statistic() -> set\n\n
     * Отсортировывает выборку в порядке возрастания sortStatistic()\n\n
     * Распределяет отсортированную выборку на интервалы buildIntervals()\n\n
     * Вывод выборки в консоль или файл в зависимости от типа потока showStatistic()\n\n
     * Вывод выборки распределенной по интервалам в консоль или файл в зависимости от типа потока showStatisticIntervals()\n
     * Рассчитывает математическое ожидание выборки statisticMean()\n\n
     * Рассчитывает средне-квадратичное отклонение выборки statisticMeanQuadraticDeviation()\n\n
     * Возможность получения максимального/минимального значения выборки statisticMinimum(Maximum)Value()\n\n
     */
    class LaboratoryStatisticEngine final {
    public:
        using statistic_type = std::vector<double>;
        using statistic_value_type = statistic_type::value_type;
        using statistic_size_type = statistic_type::size_type;

    public:
        LaboratoryStatisticEngine() = default;
        explicit LaboratoryStatisticEngine(const statistic_type &statistic) : statistic_(statistic) {}
        explicit LaboratoryStatisticEngine(statistic_type &&statistic) : statistic_(std::move(statistic)) {}

    public:
        void statistic(const statistic_type &statistic) { statistic_ = statistic; }
        void statistic(statistic_type &&statistic) noexcept { statistic_ = std::move(statistic); }
        void intervalsCount(statistic_size_type intervals_count) noexcept { intervals_count_ = intervals_count; }

        const statistic_type &statistic() const noexcept { return statistic_; };
        statistic_size_type statisticSize() const noexcept { return statistic_.size(); }
        statistic_value_type statisticMinimumValue() const noexcept { return !statistic_.empty() ? statistic_.front() : statistic_value_type{}; }
        statistic_value_type statisticMaximumValue() const noexcept { return !statistic_.empty() ? statistic_.back() : statistic_value_type{}; }
        statistic_size_type intervalsCount() const noexcept { return intervals_count_; }

        /**
         * @brief isGood
         * Проверяет в валидном ли состоянии выборка, пустая ли она
         * @return
         * Пустая ли выборка (пустая = false, не пустая = true)
         */
        bool isGood() const noexcept { return !statistic_.empty(); }

        /**
         * @brief graphStatisticData
         * Метод рассчитывает координаты практического распределения по данной выборке
         * @return
         * Возвращает координаты точек x и y
         */
        std::pair<statistic_type, statistic_type> graphStatisticData() const {
            if (statistic_.empty())
                return {};
            statistic_type x_points, y_points;
            x_points.push_back(statisticMinimumValue());
            y_points.push_back(0);
            for (statistic_size_type current = 0; current != intervals_count_; ++current) {
                auto min = intervals_border_values_[current].first, max = intervals_border_values_[current].second;
                x_points.push_back((min + max) / 2);
                y_points.push_back(relative_amount_[current]);
            }
            x_points.push_back(statisticMaximumValue());
            y_points.push_back(0);

            return std::make_pair(x_points, y_points);
        }

        /**
         * @brief graphNormalDistributionData
         * Метод рассчитывает координаты нормального распределения для данной выборки
         * @return
         * Возвращает координаты точек x и y
         */
        std::pair<statistic_type, statistic_type> graphNormalDistributionData() const {
            if (statistic_.empty())
                return {};
            statistic_type x_points, y_points;
            statistic_value_type x_min = mean_ - std::fabs(3 * quadratic_deviation_);
            statistic_value_type x_max = mean_ + std::fabs(3 * quadratic_deviation_);
            statistic_value_type h = calculateIntervalLength();
            for (auto start = x_min; start <= x_max; start += 0.01) {
                x_points.push_back(start);
                double y_point = (h * (1 / (quadratic_deviation_ * std::sqrt(2 * M_PI)))) * std::pow(M_E, -(std::pow(start - mean_, 2)/(2 * std::pow(quadratic_deviation_, 2))));
                y_points.push_back(y_point);
            }
            return std::make_pair(x_points, y_points);
        }

        /**
         * @brief sortStatistic
         * Метод сортирует по возрастанию данную статистику
         */
        void sortStatistic() {
            std::sort(statistic_.begin(), statistic_.end());
            dispersion_interval_ = statisticMaximumValue() - statisticMinimumValue();
        }

        /**
         * @brief buildIntervals
         * Метод распределяет выборку на n-количество равных интервалов и инициализирует интервалы\n
         * попавшими в них значениями из выборки
         */
        void buildIntervals() {
            if (statistic_.empty())
                return;
            sortStatistic(); // Statistic can be non-sorted
            statistic_intervals_.clear();
            relative_amount_.clear();

            statistic_value_type interval_length = calculateIntervalLength(); // Can throw exception
            intervals_border_values_ = std::vector<std::pair<statistic_value_type, statistic_value_type>>(intervals_count_);

            statistic_value_type right_border = statisticMinimumValue();
            statistic_value_type left_border = right_border;
            for (statistic_size_type interval = 0; interval != intervals_count_; ++interval) {
                if (interval != intervals_count_ - 1)
                    right_border += interval_length;
                else
                    right_border = statisticMaximumValue() + 0.01; // 0.01 is an aspect, need for a correct separating statistic
                intervals_border_values_[interval] = std::make_pair(left_border, right_border);
                left_border = right_border;
            }

            statistic_intervals_.resize(intervals_count_);
            relative_amount_.resize(intervals_count_);
            statistic_size_type current = 0;
            for (auto & interval : statistic_intervals_) {
                std::copy_if(statistic_.begin(), statistic_.end(),
                             std::back_inserter(interval),
                             [=] (statistic_value_type v) { return v >= intervals_border_values_[current].first and
                                                                   v < intervals_border_values_[current].second; });
                relative_amount_[current] = round(static_cast<double>(interval.size()) / statisticSize(), 0.001);
                ++current;
            }
        }

        /**
         * @brief eraseStatisticValue
         * Метод удаляет первое найденное value из выборки
         */        
        void eraseStatisticValue(statistic_value_type value) {
            if (!isGood())
                return;
            int finded = -1;
            for (statistic_size_type current = 0, size = statisticSize(); current != size; ++current) {
                if (statistic_[current] == value) {
                    finded = current;
                    break;
                }
            }
            if (finded == -1)
                return;
            statistic_.erase(statistic_type::const_iterator(&statistic_[finded]));
        }

        /**
         * @brief statisticSum
         * Метод рассчитывает сумму всех значений выборки
         * @return
         * сумму всех значений
         */
        statistic_value_type statisticSum() const {
            return std::accumulate(statistic_.begin(), statistic_.end(), statistic_value_type{});
        }

        /**
         * @brief statisticMean
         * Метод рассчитывает математическое ожидание данной выборки
         * @return
         * Математическое ожидание
         */
        statistic_value_type statisticMean() const {
            mean_ = statisticSum() / statisticSize();
            return mean_;
        }

        /**
         * @brief statisticMeanQuadraticDeviation
         * Метод рассчитывает среднеквадратичное отклонение данной выборки
         * @return
         * Среднеквадратичное отклонение
         */
        statistic_value_type statisticMeanQuadraticDeviation() const {
            if (std::isnan(mean_))
                return 0;
            statistic_type subdivision;
            std::transform(statistic_.begin(), statistic_.end(), std::back_inserter(subdivision),
                           [this](statistic_value_type i) { return std::pow(i - mean_, 2); });
            statistic_value_type dispersion = std::accumulate(subdivision.begin(), subdivision.end(), statistic_value_type{}) / statisticSize();
            quadratic_deviation_ = std::sqrt(dispersion);
            return quadratic_deviation_;
        }

        /**
         * @brief showStatistic
         * Метод записывает в поток вывода данные статистики
         * @return
         * Среднеквадратичное отклонение
         */
        void showStatistic(std::ostream &out, char sep = '\t') const {
            for (const auto &value : statistic_)
                out << value << sep;
            out << "(statistic size: " << statisticSize() << ")\n";
        }

         /**
         * @brief showStatisticIntervals
         * Метод записывает в поток вывода информацию об интервалах\n
         * Используй метод buildIntervals для их создания
         */
        void showStatisticIntervals(std::ostream &out) const {
            statistic_size_type current = 1;
            out.precision(4);
            for (const auto &vector : statistic_intervals_) {
                auto min = intervals_border_values_[current-1].first, max = intervals_border_values_[current-1].second;
                out << "Interval #" << current << "\t[min: " << min << ", max: "<< max << ", mid: " << (min + max) / 2 << "]:\t";
                for (const auto &item : vector) {
                    out << item << '\t';
                }
                out << "[n = " << vector.size() << ", n/N = " << relative_amount_[current-1] << "]\n";
                current++;
            }
        }

    private:
        statistic_type statistic_;
        statistic_size_type intervals_count_ {};
        statistic_value_type dispersion_interval_ {};

        std::vector<statistic_type> statistic_intervals_;
        std::vector<std::pair<statistic_value_type, statistic_value_type>> intervals_border_values_;
        statistic_type relative_amount_;

        mutable statistic_value_type mean_{}, quadratic_deviation_{};

    private:
        statistic_value_type calculateIntervalLength() const {
            if (!intervals_count_ or dispersion_interval_ <= 1e-6)
                throw std::runtime_error("Intervals count is zero!");
            return round(dispersion_interval_ / intervals_count_, 0.001);
        }

        static statistic_value_type round(statistic_value_type value, double precision = 0.01) {
            return std::round(value / precision) * precision;
        }

        static void round(statistic_value_type &value, double precision = 0.01) {
            value = std::round(value / precision) * precision;
        }
    };
}

#endif // !STATISTIC_ANALYZER_MODEL_LABORATORYENGINE_LABORATORY_ENGINE_H_
