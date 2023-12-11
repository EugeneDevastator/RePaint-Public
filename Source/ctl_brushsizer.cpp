#include "ctl_brushsizer.h"
#include <QtGui>

BrushSizer::BrushSizer(QWidget *parent)

{

    QFile logfile("d:/mhplog.log");
    logfile.open(QFile::Append);

    logfile.write("\n brushsizer init BEGIN");
    logfile.close();
    logfile.open(QFile::Append);

    /*g_Brush=new BrushData;
    g_Brush->rad_out=20;
    g_Brush->rad_in=2;*/

    ARTM = new ArtMaster();
    // ARTM->ForceSingleCore();

    logfile.write("\n brushsizer artm+");
    logfile.close();
    logfile.open(QFile::Append);

    BrushImg = QImage(128, 128, QImage::Format_ARGB32);
    // resizeImage(BrushImg, QSize(128, 128));
    rad_out = 20;
    QPainter painter(&BrushImg);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::white);
    QPointF tctr(40, 40);
    // painter.drawEllipse(tctr,30,30);
    painter.drawRect(BrushImg.rect());
    saverel = 0.5;

    this->setAttribute(Qt::WA_ContentsPropagated);
    update();

    logfile.write("\n brushsizer init FINISH");
    logfile.close();
    logfile.open(QFile::Append);
}

void BrushSizer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, BrushImg, dirtyRect);
    painter.end();
}
void BrushSizer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        Mode = 1;
    if (event->button() == Qt::RightButton)
        Mode = 2;
    if (event->button() == Qt::MiddleButton)
        Mode = 3;
}
void BrushSizer::Redraw()
{

    float hsize = BrushImg.width() / 2;
    // rad_out=g_Brush->rad_out;
    // rad_in=g_Brush->rad_in;

    QPainter painter(&BrushImg);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(QBrush(Qt::white));
    painter.fillRect(BrushImg.rect(), QBrush(Qt::white));

    // painter.end();

    QRadialGradient grad(hsize, hsize, rad_out);
    // if (Mode==3) grad.setFocalPoint(event->posF());
    grad.setColorAt(0, QColor::fromRgbF(0, 0, 0, 1));
    grad.setColorAt((float)rad_in / rad_out, QColor::fromRgbF(0, 0, 0, 1));
    if (rad_in < rad_out)
        grad.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));

    // painter.setBrush(QBrush(Qt::black));
    painter.setBrush(QBrush(grad));
    painter.drawEllipse(QPoint(hsize, hsize), rad_out, rad_out);

    d_Stroke ts;
    ts.pos1.setX(hsize);
    ts.pos1.setY(hsize);
    ts.pos2.setX(hsize);
    ts.pos2.setY(hsize);
    /*
        BrushData ViewBrush;
        ViewBrush.rad_in=g_Brush->rad_in;
        ViewBrush.rad_out=g_Brush->rad_out;
        ViewBrush.sol2op=g_Brush->sol2op;
        ViewBrush.sol=1;
        ViewBrush.compmode=QPainter::CompositionMode_SourceOver;
        ViewBrush.crv=g_Brush->crv;
        ViewBrush.col=Qt::black;
        ViewBrush.rot=rad2deg(offang);
        ViewBrush.op=1;
        ViewBrush.x2y=0.5;
        ViewBrush.scale=1;
        ARTM->DrawBrush(BrushImg,ts,ViewBrush);
      */
    g_Brush->opacity = 1;
    g_Brush->sol = 1;
    g_Brush->sol2op = 1;
    g_Brush->col = Qt::black;
    g_Brush->scale = 1;
    g_Brush->x2y = 0.5;
    // g_Brush->compmode=QPainter::CompositionMode_SourceOver;
    ActionData act;
    act.Stroke = ts;
    act.Brush.Realb = *g_Brush;
    // act.Brush.noisex=2;
    // act.Brush.noisey=2;
    act.ToolID = eBrush;

    // ARTM->ExecAction(BrushImg,act);
    // painter.begin(BrushImg);
    painter.setPen(Qt::red);
    int rx = (hsize)*cos(offang);
    int ry = (hsize)*sin(offang);

    painter.drawLine(hsize, hsize, hsize + rx, hsize + ry);
    //   painter.end();

    update();
}

void BrushSizer::mouseMoveEvent(QMouseEvent *event)
{

    float hsize = BrushImg.width() / 2;
    int rad = Dist2D(event->pos(), QPoint(hsize, hsize));

    if (Mode == 1)
    {
        rad_out = rad;
        rad_in = rad * saverel;
    }
    else if (Mode == 2)
    { // right click
        rad_in = qMin(rad - 5, rad_out - 1);
        rad_in = qMax(rad_in, 0);
        saverel = (float)(g_Brush->rad_in / g_Brush->rad_out);
        // saverel=qMin(saverel,(float)1);
    }
    else if (Mode == 3)
    {
        int ax = -event->pos().x() + hsize;
        int ay = -event->pos().y() + hsize;
        offang = AtanXY(ax, ay);
        if (rad > hsize * 1.6)
            offang = Discrete(offang, M_PI_4 * 0.5);

        g_Brush->resangle = rad2deg(offang);
    }
    if (rad_in > rad_out)
        rad_in = rad_out;

    g_Brush->rad_out = rad_out;
    g_Brush->rad_in = rad_in;

    this->Redraw();
    emit SendSize(rad_out);
    emit SendRadRel(saverel);
}
