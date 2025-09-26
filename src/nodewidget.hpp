#ifndef NODEWIDGET_H
#define NODEWIDGET_H

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <algorithm>

#include "analysis.hpp"
#include "chartwidget.hpp"
#include "graph.hpp"

class NodeWidget : public QWidget {
  Q_OBJECT

 public:
  NodeWidget(QWidget* parent);
  ~NodeWidget();

 private:
  ChartWidget* charts_;     /*!< Chart widget */
  QVBoxLayout* mainLayout_; /*!< Widget main layout*/
  QGridLayout* textLayout_; /*!< Layout for labels */

  QLabel* title_;                 /*!< Screen title */
  QLabel* maxUtilText_;           /*!< Max utilation text */
  QLabel* minUtilText_;           /*!< Min utilation text */
  QLabel* sentText_;              /*!< Number of sent packets text */
  QLabel* droppedText_;           /*!< Number of dropped packets text */
  QLabel* receivedText_;          /*!< Number of received packets text */
  QLabel* processTimeText_;       /*!< Average process time for packet text */
  QLabel* deliveredMeanText_;     /*!< Average of all delivery times text */
  QLabel* deliveredVarianceText_; /*!< Variance of all delivery times text */
  QLabel* maxUtil_;               /*!< Max utilation */
  QLabel* minUtil_;               /*!< Min utilation */
  QLabel* sent_;                  /*!< Number of sent packets */
  QLabel* dropped_;               /*!< Number of dropped packets */
  QLabel* received_;              /*!< Number of received packets */
  QLabel* processTime_;           /*!< Average process time for packet */
  QLabel* deliveredMean_;         /*!< Average of all delivery times */
  QLabel* deliveredVariance_;     /*!< Variance of all delivered */

  Analysis* analysis_;
  int maxUtilValue_ = 0;
  int minUtilValue_ = 0;

  void initializeText();

 public:
  void updateValues(Graph* graph);
  void reset();
};

#endif  // NODEWIDGET_H