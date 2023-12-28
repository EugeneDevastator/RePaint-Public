#include "bctl_dblslider.h"
#include <math.h>
#include <QLinearGradient>

DialSliderWidget::DialSliderWidget(DialModel *model, QWidget *parent) : bctl_ImageWidget(parent) {
    Model = model;
    ActivePick = -1;
    DsRange = 0.25;
    clipminF = 0;
    clipmaxF = 1;
    jitter = 0.0;
    grad = new QLinearGradient(0, 3, this->width(), 3);
    // if (clipmaxF>clipmaxF) cp=0;
    grad->setColorAt(0.0, QColor::fromHslF(0, 0, 0, 1));
    grad->setColorAt(1.0, QColor::fromHslF(0, 0, 1, 1));
    orient = 0; // 0 is horisontal 1 is vertical
    ViewImage = QImage(this->size(), QImage::Format_RGB444);
    sliderrad = 2;
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    MaxCol = QColor::fromRgbF(1, 1, 1, 1);
    MinCol = QColor::fromRgbF(0, 0, 0, 1);
    shade.setRgb(144, 144, 144);
    hlite.setRgb(250, 250, 250);
    midtone.setRgb(240, 240, 240);
}

void DialSliderWidget::Redraw() {
    quint8 Soff = 2; // oldSlider offset from the borders
    QPainter painter(&ViewImage);
    if (orient == 0) {
        painter.setPen(Qt::SolidLine);
        // float top=qMax(clipmaxF,clipminF);
        // float bot=qMin(clipmaxF,clipminF);
        // QLinearGradient grad(this->width()*clipminF,3,this->width()*clipmaxF,3);
        grad->setStart(0, 3);
        grad->setFinalStop(this->width(), 3);
        painter.setBrush(*grad);
        painter.drawRect(0, 0, this->width(), this->height());
        // painter.drawRect(this->rect());
        // float wd=this->width()*(clipmaxF-clipminF);

        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);
        if (jitter > 0)
            painter.drawRect(0, 0, this->width() * jitter, 7);

        painter.setPen(Qt::NoPen);
        QRect indr;
        painter.setBrush(MinCol);
        indr = QRect(this->width() * clipminF - sliderrad, Soff, sliderrad * 2, this->height() - Soff * 2 - 1);
        painter.drawRect(indr);

        DrawFrame(&ViewImage, &painter, indr, shade, midtone);

        painter.setPen(Qt::NoPen);
        painter.setBrush(MaxCol);
        indr = QRect(this->width() * clipmaxF - sliderrad, Soff, sliderrad * 2, this->height() - Soff * 2 - 1);
        painter.drawRect(indr);

        DrawFrame(&ViewImage, &painter, indr, midtone, shade);
    } else {
        painter.setPen(Qt::NoPen);
        grad->setStart(3, this->height());
        grad->setFinalStop(3, 0);
        painter.setBrush(*grad);
        painter.drawRect(0, 0, this->width(), this->height());

        painter.setPen(Qt::SolidLine);
        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);
        if (jitter > 0)
            painter.drawRect(0, this->height() * (1 - jitter), 7, this->height());

        // painter.setPen(Qt::white);
        painter.setBrush(MinCol);
        painter.setPen(Qt::NoPen);
        QRect indr;

        indr = QRect(Soff, this->height() * (1 - clipminF) - sliderrad, this->width() - Soff * 2 - 1, sliderrad * 2);
        painter.drawRect(indr);

        DrawFrame(&ViewImage, &painter, indr, shade, midtone);

        painter.setPen(Qt::NoPen);
        painter.setBrush(MaxCol);
        indr = QRect(Soff, this->height() * (1 - clipmaxF) - sliderrad, this->width() - Soff * 2 - 1, sliderrad * 2);
        painter.drawRect(indr);

        DrawFrame(&ViewImage, &painter, indr, midtone, shade);
    }
    if (Model) {
        painter.setPen(Qt::black);
        // painter.drawRect(this->width()*clipmaxF,0,-4,this->height()-1);
        painter.drawText(this->width() * 0.5 - 25+1, +1, 50, this->height(),
                         Qt::AlignVCenter | Qt::AlignHCenter,
                //QString::number((quint8)((int)255 * clipmaxF)));
                         QString::number(Model->GetValueAtMax(), 'f', 2));
        painter.setPen(Qt::white);
        // painter.drawRect(this->width()*clipmaxF,0,-4,this->height()-1);
        painter.drawText(this->width() * 0.5 - 25, 0, 50, this->height(),
                         Qt::AlignVCenter | Qt::AlignHCenter,
                //QString::number((quint8)((int)255 * clipmaxF)));
                         QString::number(Model->GetValueAtMax(), 'f', 2));
    }

    // draw border frame;

    painter.setPen(Qt::NoPen);

    /*  painter.setBrush(shade);
      painter.drawRect(0,0,this->width(),1);
      painter.drawRect(0,0,1,this->height());

      painter.setBrush(midtone);
      painter.drawRect(this->width()-1,1,1,this->height());
      painter.drawRect(1,this->height()-1,this->width(),1);
      */
    DrawFrame(&ViewImage, &painter, 0, 0, this->width(), this->height(), shade, midtone);
    update();
    emit Repainted();
}
// void DialSliderWidget::paintEvent(QPaintEvent *event){}


void DialSliderWidget::mousePressEvent(QMouseEvent *event) {

    if (event->buttons() == Qt::LeftButton)
        ActivePick = 1; // maximum
    else if (event->buttons() == Qt::RightButton)
        ActivePick = 0; // minimum
    else if (event->buttons() == Qt::MiddleButton)
        ActivePick = 2; // jitter

    ParsePoint(event->pos());
    // QApplication::processEvents(QEventLoop::AllEvents,10);
}

void DialSliderWidget::ParsePoint(QPoint pos) {
    if (ActivePick > -1) {
        float val;
        if (orient == 0) {
            val = (float) ((float) pos.x() / ViewImage.width());
            val = qMin(val, (float) 1);
            val = qMax(val, (float) 0);

            if ((pos.y() > this->height() * 2) | (pos.y() < -this->height())) {
                // float drange=0.25;
                val = val / DsRange;
                val = round(val) * DsRange;
            }
        } else // vertical orient;
        {
            val = (float) ((float) pos.y() / ViewImage.height());
            val = qMin(val, (float) 1);
            val = qMax(val, (float) 0);
            val = 1 - val;
            if ((pos.x() > this->width() * 2) | (pos.x() < -this->width())) {
                // float drange = 0.25;
                val = val / DsRange;
                val = round(val) * DsRange;
            }
        }

        if (ActivePick == 1) {
            clipmaxF = val;
        }
        if (ActivePick == 0) {
            clipminF = val;
        }
        if (ActivePick == 2) {
            jitter = val;
        }

        emit ValChange(val);
        //   emit AllValChange(clipmaxF, clipminF, jitter);
    }
    Redraw();
    update();
    QApplication::processEvents(QEventLoop::AllEvents, 50);
}

void DialSliderWidget::change() {
    // emit AllValChange(clipmaxF, clipminF, jitter);
}

void DialSliderWidget::SetValues(float maxf, float minf, float jit) {
    clipmaxF = maxf;
    clipminF = minf;
    jitter = jit;
    Redraw();
    update();
}

void DialSliderWidget::tabletEvent(QTabletEvent *event) {

    if (event->type() == QEvent::TabletPress) {

        if (event->pointerType() == QTabletEvent::Eraser)
            ActivePick = 2;
        else
            ActivePick = 1;
    }
    if (event->type() == QEvent::TabletRelease) {
        ActivePick = -1;
    }

    if (event->type() == QEvent::TabletMove) {
        ParsePoint(event->pos());
    }
}

void DialSliderWidget::mouseMoveEvent(QMouseEvent *event) {

    ParsePoint(event->pos());
}

void DialSliderWidget::mouseReleaseEvent(QMouseEvent *event) {
    emit ValConfirm(clipmaxF);
    ActivePick = -1;
}

void DialSliderWidget::resizeEvent(QResizeEvent *event) {
    resizeImage(&ViewImage, this->size());
    int thin = qMin(this->width(), this->height());
    sliderrad = thin * 0.25 * 0.5;
    Redraw();
    update();
}

void DialSliderWidget::DrawFrame(QImage *img, QPainter *pnt, QRect rect, QColor SHD, QColor HL) {
    DrawFrame(img, pnt, rect.x(), rect.y(), rect.width(), rect.height(), SHD, HL);
}

void DialSliderWidget::DrawFrame(QImage *img, QPainter *pnt, int x, int y, int w, int h, QColor SHD, QColor HL) {

    //  QPainter pnt(img);
    pnt->setPen(Qt::NoPen);
    pnt->setBrush(SHD);

    pnt->drawRect(x, y, w, 1);
    pnt->drawRect(x, y, 1, h);

    pnt->setBrush(HL);
    pnt->drawRect(x + w - 1, y + 1, 1, h - 1);
    pnt->drawRect(x + 1, y + h - 1, w - 1, 1);
}
