#ifndef QLED_H
#define QLED_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class Qled : public QLabel {
  Q_OBJECT
  Qled(const Qled&) = delete;
  Qled& operator=(const Qled&) = delete;
 public Q_SLOTS:
  void setOn() {
    onPix.fill(Qt::red);
    setPixmap(onPix);
  }
  void setOff() {
    onPix.fill(Qt::black);
    setPixmap(onPix);
  }
  void setPower(bool val) {
    if(val) setOn();
    else setOff();
  }
 public:
  virtual ~Qled() = default;
  explicit Qled(QWidget* parent = nullptr)
    : QLabel(parent)
    , onPix(48, 48) {
    onPix.fill(Qt::black);
    setPixmap(onPix);
    setScaledContents(true);
  }
 private:
  QPixmap onPix;
};

#endif // QLED_H
