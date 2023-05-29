# STATISTIC ANALYZER by tonitaga

---

## Contents

1. [Developers](#developers)
2. [Project idea](#project-idea)
3. [How it looks like?](#how-it-looks-like)
4. [QCustomPlot](#qcustomplot)

---

<p align="center">
<img src="misc/images/graph.png" style="width: 35vw; min-width: 300px;" />
</p>

---

### Developers:
- Gubaydullin Nurislam

---

### Project idea

* The main idea was to automate the performance of laboratory work at the Kazan Aviation Institute on the subject of Fundamentals of Mechanical Engineering Technology
* The implementation of this application was prompted by the love of his classmates and the love of mathematics

---

### How it looks like?

* The file with the extension is taken as a basis .txt, where the sample data is written separated by a space or on a new line
* The application reads the sample from the file and calculates the basic data of mathematical statistics for the current sample

<p align="center">
<img src="misc/images/loaded_statistic.png" style="width: 35vw; min-width: 400px;" />
</p>

* By clicking the "Разбить выборку" button, having previously selected a little higher by how many intervals, you can get a graph of the practical distribution over the intervals of the current sample

<p align="center">
<img src="misc/images/build_intervals.png" style="width: 35vw; min-width: 400px;" />
</p>

* By clicking the "Нормальное распределение" button, having built a practical distribution graph in advance, a graph of the reduced (to the practical graph) normal distribution will be built

<p align="center">
<img src="misc/images/normal_distribution.png" style="width: 35vw; min-width: 400px;" />
</p>

* By clicking the "Получить интервалы" button, after splitting the sample into intervals in advance, you can get a file with intervals in the format .txt

<p align="center">
<img src="misc/images/save_intervals.png" style="width: 35vw; min-width: 400px;" />
</p>

* By clicking the "Сортированная статистика" button, you can get a file with a sorted selection in the format .txt

<p align="center">
<img src="misc/images/sorted_statistic.png" style="width: 35vw; min-width: 400px;" />
</p>

---

### QCustomPlot

* Graphs are built using the QCustomPlot library
* Developers' website [QCustomPlot](https://www.qcustomplot.com/)

---

#### © tonitaga (Gubaydullin Nurislam) 29.05.2023