#include "Brushes.h"
#include "BrushEngine/BrushStamp.hpp"
#include <QPainter>
#include <math.h>
#include <QtGlobal>
#include <QObject>
#include <QMatrix>
#include <signal.h>
#include <QtWidgets/QWidget>
#include <QByteArray>

// EGlobals::str1="asd";
QString EGlobals::str1 = "asdasd";
// QString RESPATH::clear();
// QString RESPATH::append(RESPATH);
// static QString RESPATH(QApplication::applicationDirPath());
QList<QString> EGlobals::PipeNames;

void PackedFloat::SetVal(double val) {
    IntVal = (qint16) (floor(val));
    FVal = (quint8) floor((val - floor(val)) * 255);
}

qreal PackedFloat::GetVal() {
    return (qreal) (IntVal + FVal / 255.0);
}

void d_PointF::SetByQPointF(QPointF src) {
    xpos.SetVal(src.x());
    ypos.SetVal(src.y());
}

QPointF d_PointF::ToPointF() {
    QPointF res;
    res.setX(xpos.GetVal());
    res.setY(ypos.GetVal());
    return res;
}

qreal d_PointF::x() {
    return xpos.GetVal();
}

qreal d_PointF::y() {
    return ypos.GetVal();
}

QString DSZstring(QByteArray ba) {
    QString st;
    QDataStream tds(&ba, QIODevice::ReadOnly);
    tds >> st;
    return st;
}

QByteArray SZstring(QString st) {
    QByteArray *ba = new QByteArray(1, 'c');

    QDataStream tds(ba, QIODevice::ReadWrite);
    tds << st;

    return *ba;
}

QByteArray LayerData::Serialize() {

    QByteArray qba;
    QDataStream ba(&qba, QIODevice::ReadWrite);
    Serialize(&ba);

    return qba;
}

void LayerData::Serialize(QDataStream *ba) {

    *ba << Opacity;
    *ba << IsVisible;
    *ba << blendmode;
    *ba << presop;
    *ba << IsLocked;
    *ba << layerName;
}

void LayerData::DeSerialize(QByteArray src) {
    QDataStream ba(&src, QIODevice::ReadWrite);
    // putting in a stroke
    DeSerialize(&ba);
}

void LayerData::DeSerialize(QDataStream *ba) {
    // QDataStream ba(src,QIODevice::ReadWrite);
    //  putting in a stroke
    *ba >> Opacity;
    *ba >> IsVisible;
    *ba >> blendmode;
    *ba >> presop;
    *ba >> IsLocked;
    *ba >> layerName;
}

QByteArray d_Stroke::Serialize() {

    QByteArray qba;
    QDataStream ba(&qba, QIODevice::ReadWrite);
    // putting in a laction
    packpos1.SetByQPointF(pos1);
    packpos2.SetByQPointF(pos2);
    ba << packpos1.xpos.IntVal;
    ba << packpos1.xpos.FVal;
    ba << packpos1.ypos.IntVal;
    ba << packpos1.ypos.FVal;
    ba << packpos2.xpos.IntVal;
    ba << packpos2.xpos.FVal;
    ba << packpos2.ypos.IntVal;
    ba << packpos2.ypos.FVal;

    return qba;
}

void d_Stroke::Serialize(QDataStream *ba) {

    packpos1.SetByQPointF(pos1);
    packpos2.SetByQPointF(pos2);
    *ba << packpos1.xpos.IntVal;
    *ba << packpos1.xpos.FVal;
    *ba << packpos1.ypos.IntVal;
    *ba << packpos1.ypos.FVal;
    *ba << packpos2.xpos.IntVal;
    *ba << packpos2.xpos.FVal;
    *ba << packpos2.ypos.IntVal;
    *ba << packpos2.ypos.FVal;
}

void d_Stroke::DeSerialize(QByteArray *src) {
    QDataStream ba(src, QIODevice::ReadWrite);
    DeSerialize(&ba);
}

void d_Stroke::DeSerialize(QDataStream *ba) {
    // QDataStream ba(src,QIODevice::ReadWrite);
    //  putting in a stroke
    *ba >> packpos1.xpos.IntVal;
    *ba >> packpos1.xpos.FVal;
    *ba >> packpos1.ypos.IntVal;
    *ba >> packpos1.ypos.FVal;
    *ba >> packpos2.xpos.IntVal;
    *ba >> packpos2.xpos.FVal;
    *ba >> packpos2.ypos.IntVal;
    *ba >> packpos2.ypos.FVal;
    pos1 = packpos1.ToPointF();
    pos2 = packpos2.ToPointF();
}

QByteArray ActionData::Serialize() {
    QByteArray *qba = new QByteArray('c', 1);
    qba->clear();
    QDataStream ba(qba, QIODevice::ReadWrite);

    ba << Stroke.Serialize();
    ba << Brush.Serialize();

    ba << ToolID;
    ba << layer;

    return *qba;
}

void ActionData::DeSerialize(QByteArray src) {
    QDataStream ba(&src, QIODevice::ReadWrite);
    // putting in a stroke
    ba >> Stroke.packpos1.xpos.IntVal;
    ba >> Stroke.packpos1.xpos.FVal;
    ba >> Stroke.packpos1.ypos.IntVal;
    ba >> Stroke.packpos1.ypos.FVal;
    ba >> Stroke.packpos2.xpos.IntVal;
    ba >> Stroke.packpos2.xpos.FVal;
    ba >> Stroke.packpos2.ypos.IntVal;
    ba >> Stroke.packpos2.ypos.FVal;
    // now brush
    Stroke.pos1 = Stroke.packpos1.ToPointF();
    Stroke.pos2 = Stroke.packpos2.ToPointF();

    Brush.DeSerialize(&ba);

    ba >> ToolID;
    ba >> layer;
}

//--------------------------  SECTION SERIALIZATION;
void StrokeSection::DeSerialize(QByteArray src) {
    QDataStream ba(&src, QIODevice::ReadWrite);
    Stroke.DeSerialize(&ba);
    Brush.DeSerialize(&ba);
    BrushFrom.DeSerialize(&ba);
    ba >> ToolID;
    ba >> BrushID;
    ba >> NoiseID;
    ba >> Noisemode;

    ba >> startseed;
    ba >> layer;
    ba >> spacing;
    ba >> scatter; // fixit!
    ba >> rRadout;
    ba >> rRadrel;
    ba >> rScale;
    ba >> rScaleRel;
    ba >> rAngle;
    ba >> rSpacing;
    ba >> rSpread;
    ba >> rOp;
    ba >> rSol;
    ba >> rSol2;
    ba >> rCrv;
    ba >> rCop;
    ba >> rPwr;
    ba >> rHue;
    ba >> rSat;
    ba >> rLit;
}

QByteArray StrokeSection::Serialize() {
    QByteArray *qba = new QByteArray('c', 1);
    qba->clear();
    QDataStream ba(qba, QIODevice::ReadWrite);
    Stroke.Serialize(&ba);
    Brush.Serialize(&ba);
    BrushFrom.Serialize(&ba);
    ba << ToolID;
    ba << BrushID;
    ba << NoiseID;
    ba << Noisemode;
    ba << startseed;
    ba << layer;
    ba << spacing;
    ba << scatter; // fixit!
    ba << rRadout;
    ba << rRadrel;
    ba << rScale;
    ba << rScaleRel;
    ba << rAngle;
    ba << rSpacing;
    ba << rSpread;
    ba << rOp;
    ba << rSol;
    ba << rSol2;
    ba << rCrv;
    ba << rCop;
    ba << rPwr;
    ba << rHue;
    ba << rSat;
    ba << rLit;
    return *qba;
}

// -------------------------  LACTION SERIALIZATION

QByteArray LayerAction::Serialize() {
    QByteArray qba;
    QDataStream ds(&qba, QIODevice::ReadWrite);
    ds << ActID;
    ds << layer;
    ds << layerto;
    ds << bm;
    ds << op;
    ds << vis;
    ds << rect;

    return qba;
}

void LayerAction::DeSerialize(QByteArray src) {
    QDataStream ds(&src, QIODevice::ReadWrite);

    ds >> ActID;
    ds >> layer;
    ds >> layerto;
    ds >> bm;
    ds >> op;
    ds >> vis;
    ds >> rect;
}

void BrushDialConfig::Tobase() {
    clipmax = basemax;
    clipmin = basemin;
    outmax = basemax;
    outmin = basemin;
}

void BrushDialConfig::Unclip() {
    clipmax = basemax;
    clipmin = basemin;
}

float BrushDialConfig::GetCVal(float inp) {
    // float brange=basemax-basemin;
    // float relpos=(inp-basemin)/brange; // from 0 to 1;

    float cliprange = clipmax - clipmin;
    // float clippos=clipmin+relpos*cliprange;
    float cpos = (inp - clipmin) / cliprange;
    float crpos1 = qMax(cpos, (float) 0); // clipped rel pos;
    float crpos2 = qMin(crpos1, (float) 0);
    float outrange = outmax - outmin;
    float outpos = outrange * crpos2 + outmin;

    return outpos;
}

float BrushDialConfig::GetTVal(float inp) {
    // gets transoutputted val. in the base out range.
    //  base determines base input like -60 +60 for angles
    //  clip determines range like form -10 to 20
    //  outs are considered to base, so 60 will be 1, -60 = 0;

    // we shrink inputted value into clipped range;
    // output is generated according to output range.

    float brange = basemax - basemin;
    float crange = clipmax - clipmin;
    float brpos = (inp - basemin) / brange;
    float clpos = crange * brpos + clipmin;
    float brpos2 = (clpos - basemin) / brange;
    float outrange = outmax - outmin;
    float outpos = outrange * brpos2 + outmin;
    return outpos;
}

void InitCore() {
    // B1=QImage("d:\b1.png");
}
//----------------------------------- L I N E ---------------------------------

void resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    //    if (image->size() == newSize)
    //     return;
    QImage newImage(newSize, QImage::Format_ARGB32_Premultiplied);

    // newImage.fill(qRgba(255,255,255,0));
    QPainter painter(&newImage);
    painter.drawImage(0, 0, *image);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor::fromRgbF(1, 1, 1, 0));
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawRect(newImage.rect());

    *image = newImage;
}

void rescaleImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_ARGB32_Premultiplied);

    // newImage.fill(qRgba(255,255,255,0));
    QPainter painter(&newImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor::fromRgbF(1, 1, 1, 0));
    painter.drawImage(newImage.rect(), *image);

    // painter.drawRect(newImage.rect());

    *image = newImage;
}

// Smart color

// MATH OPERATIONS
float Clamp(float a, float max) {
    float v = (a < 0) ? 0 : (a < max) ? a
                                      : max;
    return v;
}

qreal Dist2D(QPointF pos1, QPointF pos2) {
    return (qreal) sqrt(
            (qreal) ((pos1.x() - pos2.x()) * (pos1.x() - pos2.x()) + (pos1.y() - pos2.y()) * (pos1.y() - pos2.y())));
}

void MulPos(QPointF *pos1, float mul) {
}

float AtanXY(float x, float y) {

    double ang;
    signed int sg;
    sg = -1;
    if (y > 0)
        sg = 0;
    if (x == 0 && y == 0)
        ang = 0;
    else if (x == 0)
        ang = M_PI_2 + (sg + 1) / 2 * M_PI;
    else if (x > 0 && y < 0)
        ang = M_PI * 2 + atan(y / x);
    else if (x < 0)
        ang = M_PI + atan(y / x);
    else
        ang = atan(y / x);

    // ang=ang*180/M_PI;
    return ang - M_PI;
    // TODO: Add your source code here
}

float rad2deg(float rads) {
    return rads * 180 / M_PI;
}

float Discrete(float inp, float range) {
    inp = inp / range;
    return round(inp) * range;
}

qreal RngConv(qreal inval, qreal inmin, qreal inmax, qreal outmin, qreal outmax) {
    qreal inrange = inmax - inmin;
    qreal inrel = (inval - inmin) / inrange;
    inrel = qMax(inrel, (qreal) 0);
    inrel = qMin(inrel, (qreal) 1);
    qreal outrange = outmax - outmin;
    return (outrange * inrel + outmin);
}

bool isnear(float val, float val2, float range) {
    if (fabs(val - val2) < range)
        return true;
    else
        return false;
}

QPoint WHratio(QPoint wh) {
    // int dif=abs(wh.x()-wh.y());
    int min = qMin(wh.x(), wh.y());
    float dif = 2;
    while (dif <= min) {
        if ((wh.x() / dif != floor(wh.x() / dif)) | (wh.y() / dif != floor(wh.y() / dif))) {
            dif++;
        } else
            return WHratio(QPoint(wh.x() / dif, wh.y() / dif));
    }
    return wh;
}

// -------------------------- TABLET CALIBRATION PARAMETERS ---------------------

QRgb QcToRGB(QColor col) {
    QRgb res;
    res = qRgba(col.red(), col.green(), col.blue(), col.alpha());
    return res;
}

qreal Blend2(qreal from, qreal to, qreal k) {
    return (from + (to - from) * k);
}

QRgb BlendRGB(QRgb from, QRgb to, qreal k) {
    QRgb bc;
    bc = qRgba(
            Blend2(qRed(from), qRed(to), k),
            Blend2(qGreen(from), qGreen(to), k),
            Blend2(qBlue(from), qBlue(to), k),
            Blend2(qAlpha(from), qAlpha(from), k));
    return bc;
}

QColor BlendQCOL(QColor from, QColor to, qreal k) {
    QColor bc;
    bc.fromRgbF(Blend2(from.redF(), to.redF(), k),
                Blend2(from.greenF(), to.greenF(), k),
                Blend2(from.blueF(), to.blueF(), k));
    return bc;
}

QPointF CalcLastPos(StrokeSection Sect) {

    float stdist = Dist2D(Sect.Stroke.pos1, Sect.Stroke.pos2);
    qint16 rad = Sect.BrushFrom.ClientStamp.rad_out * Sect.BrushFrom.ClientStamp.scale;
    qint16 endradius = Sect.Brush.ClientStamp.rad_out * Sect.BrushFrom.ClientStamp.scale;

    qreal dx = Sect.Stroke.pos1.x() - Sect.Stroke.pos2.x();
    qreal dy = Sect.Stroke.pos1.y() - Sect.Stroke.pos2.y();
    qreal x2r = dx / stdist;
    qreal y2r = dy / stdist;

    qreal curlen = 0;                                            // offset; // initial offset must be = -rad*sect.spacing
    qreal nextrad = rad + (curlen * (endradius - rad) /
                           stdist); //((stdist*rad)+(rad2-rad)*curlen)/(stdist+rad-rad2); // calculating next radius`
    qreal nextlen = curlen +
                    nextrad * Sect.spacing;             // nextlen is location of next centre measured from stroke start

    qreal rnflw = 0.0;  //(qrand()/32767.0*rrang*2)-rrang;
    qreal rnside = 0.0; //(qrand()/32767.0*rrang*2)-rrang;

    QPointF dotpos1;
    QPointF dotpos2;
    dotpos2 = Sect.Stroke.pos2;

    rnflw = 0.0;  //(qrand()/32767.0*rrang*2)-rrang;
    rnside = 0.0; //(qrand()/32767.0*rrang*2)-rrang;

    if (nextlen < stdist) {
        while (nextlen < stdist) {
            qreal k = nextlen / stdist;
            dotpos1.setX(Sect.Stroke.pos2.x() + nextlen * x2r - rnflw * y2r + rnside * x2r);
            dotpos1.setY(Sect.Stroke.pos2.y() + nextlen * y2r + rnflw * x2r + rnside * y2r);

            nextlen = nextlen + nextrad * Sect.spacing;
        }

        return dotpos1;
    } else
        return Sect.Stroke.pos2;
}

qreal SeqRnd(quint16 seed, quint16 N, qreal range) {
    quint32 Sseed = seed + N * 13;
    qreal x;
    quint32 a = 64136401;
    quint32 c = 5;
    quint32 m = 25500;

    x = (a * Sseed * Sseed * Sseed) % m;
    return x / 25500.0;
}

qreal RawRnd(quint16 seed, qreal range) {
    qreal x;
    quint32 a = 64136401;
    quint32 c = 5;
    quint32 m = 25500;

    x = (a * seed * seed * seed) % m;
    return x / 25500.0;
}

quint16 RND(quint16 range) {
    quint32 Sseed =
            QTime::currentTime().hour() * 10000 + QTime::currentTime().second() * 100 + QTime::currentTime().minute();
    qreal x;
    quint32 a = 64136401;
    // quint32 c=5;
    quint32 m = range;

    x = (a * Sseed * Sseed * Sseed) % m;
    return x;
}

qreal RNDf(qreal range) {
    Rnd_Counter += 675;
    quint32 Sseed =
            QTime::currentTime().minute() * 10000 + QTime::currentTime().second() * 100 + QTime::currentTime().msec() +
            Rnd_Counter;
    return RawRnd(Sseed / 3, 0) * range;
}

ClientBrushStamp BlendBrushes(ClientBrushStamp Bfrom, ClientBrushStamp Bto, float k) {
    ClientBrushStamp bbr;
    //(f+  (t-f)    *k)

    bbr.rad_in = Blend2(Bfrom.rad_in, Bto.rad_in, k);
    bbr.rad_out = Blend2(Bfrom.rad_out, Bto.rad_out, k);
    bbr.crv = Blend2(Bfrom.crv, Bto.crv, k);
    bbr.resangle = Blend2(Bfrom.resangle, Bto.resangle, k);
    // bbr.resangle=Blend2(Bfrom.resangle,Bto.resangle,k);
    bbr.pwr = Blend2(Bfrom.pwr, Bto.pwr, k);
    bbr.x2y = Blend2(Bfrom.x2y, Bto.x2y, k);
    bbr.scale = Blend2(Bfrom.scale, Bto.scale, k);
    bbr.opacity = (Blend2(Bfrom.opacity, Bto.opacity, k));
    bbr.cop = Blend2(Bfrom.cop, Bto.cop, k);

    bbr.sol = Blend2(Bfrom.sol, Bto.sol, k);
    bbr.sol2op = Blend2(Bfrom.sol2op, Bto.sol2op, k);

    bbr.sol2op = Blend2(Bfrom.sol2op, Bto.sol2op, k);
    //  bbr.noisex=Blend2(Bfrom.noisex,Bto.noisex,k);
    //  bbr.noisey=Blend2(Bfrom.noisey,Bto.noisey,k);

    // QColor LColor=BlendQCOL(Bfrom.col,Bto.col,k);
    bbr.col = BlendRGB(Bfrom.col, Bto.col, k); // qRgba(LColor.red(),LColor.green(),LColor.blue(),qAlpha(bbr.col));

    bbr.seed = Bfrom.seed + k * 10;

    bbr.NoiseID = Bfrom.NoiseID;
    bbr.MaskID = Bfrom.MaskID;
    bbr.pipeID = Bfrom.pipeID;
    bbr.bmidx = Bfrom.bmidx;
    bbr.preserveop = Bfrom.preserveop;

    return bbr;
}

