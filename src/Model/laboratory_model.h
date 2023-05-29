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

#ifndef STATISTIC_ANALYZER_MODEL_LABORATORY_MODEL_H_
#define STATISTIC_ANALYZER_MODEL_LABORATORY_MODEL_H_

#include "DataReader/data_reader.h"
#include "LaboratoryEngine/laboratory_engine.h"

#include <memory>

namespace s21 {
    /**
     * @class LaboratoryModel class\n
     * Объединяет функционал NormalDistributionStatisticReader и LaboratoryStatisticEngine классов\n
     * Moveable, non-copyable, final class
     */
    class LaboratoryModel final {
        using statistic_type = LaboratoryStatisticEngine::statistic_type;
        using statistic_value_type = LaboratoryStatisticEngine::statistic_value_type;
        using statistic_size_type = LaboratoryStatisticEngine::statistic_size_type;

    public:
        void readStatisticFromFile(const std::string &path) {
            auto statistic = s21::NormalDistributionStatisticReader::read(path);
            engine_->statistic(std::move(statistic));
        }

        void sortStatistic() { engine_->sortStatistic(); }

        void buildStatisticIntervals(const statistic_size_type &intervals_count) {
            engine_->intervalsCount(intervals_count);
            engine_->buildIntervals();
        }

        void showStatistic(std::ostream &out, char sep = '\t') const { engine_->showStatistic(out, sep); }
        void showStatisticIntervals(std::ostream &out) const { engine_->showStatisticIntervals(out); }

        statistic_value_type getStatisticMeanValue() const { return engine_->statisticMean(); }
        statistic_value_type getStatisticMeanQuadraticDeviationValue() const { return engine_->statisticMeanQuadraticDeviation(); }
        statistic_value_type getStatisticSum() const { return engine_->statisticSum(); }
        statistic_size_type  getStatisticSize() const noexcept { return engine_->statisticSize(); }
        statistic_size_type  getStatisticIntervalsCount() const noexcept { return engine_->intervalsCount(); }
        const statistic_type &getStatistic() const noexcept { return engine_->statistic(); }
        statistic_value_type getStatisticMinimumValue() const noexcept { return engine_->statisticMinimumValue(); }
        statistic_value_type getStatisticMaximumValue() const noexcept { return engine_->statisticMaximumValue(); }
        std::pair<statistic_type, statistic_type> getGraphStatisticData() const { return engine_->graphStatisticData(); }
        std::pair<statistic_type, statistic_type> getGraphNormalDistributionData() const { return engine_->graphNormalDistributionData(); }

        bool statisticIsGood() const noexcept { return engine_->isGood(); }

        void eraseStatisticValue(const statistic_value_type &value) { engine_->eraseStatisticValue(value); }


    private:
        std::unique_ptr<LaboratoryStatisticEngine> engine_{new LaboratoryStatisticEngine};
    };
}

#endif // !STATISTIC_ANALYZER_MODEL_LABORATORY_MODEL_H_
