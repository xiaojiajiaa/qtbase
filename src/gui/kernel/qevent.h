/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QEVENT_H
#define QEVENT_H

#include <QtGui/qtguiglobal.h>

#include <QtCore/qcoreevent.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qlist.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qpointer.h>
#include <QtCore/qstring.h>
#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>
#include <QtGui/qpointingdevice.h>
#include <QtGui/qregion.h>
#include <QtGui/qvector2d.h>
#include <QtGui/qwindowdefs.h>

#if QT_CONFIG(shortcut)
#  include <QtGui/qkeysequence.h>
#endif

QT_BEGIN_NAMESPACE

class QFile;
class QAction;
class QPointerEvent;
class QScreen;
#if QT_CONFIG(gestures)
class QGesture;
#endif

class Q_GUI_EXPORT QInputEvent : public QEvent
{
public:
    explicit QInputEvent(Type type, const QInputDevice *m_dev, Qt::KeyboardModifiers modifiers = Qt::NoModifier);
    ~QInputEvent();
    const QInputDevice *device() const { return m_dev; }
    QInputDevice::DeviceType deviceType() const { return m_dev ? m_dev->type() : QInputDevice::DeviceType::Unknown; }
    inline Qt::KeyboardModifiers modifiers() const { return modState; }
    inline void setModifiers(Qt::KeyboardModifiers amodifiers) { modState = amodifiers; }
    inline ulong timestamp() const { return ts; }
    inline void setTimestamp(ulong atimestamp) { ts = atimestamp; }

protected:
    const QInputDevice *m_dev = nullptr;
    Qt::KeyboardModifiers modState = Qt::NoModifier;
    ulong ts;
    qint64 m_extra = 0; // reserved, unused for now
};

namespace QTest {
    class QTouchEventSequence; // just for the friend declaration below
}

class Q_GUI_EXPORT QEventPoint
{
    Q_GADGET
public:
    enum State : quint8 {
        Unknown     = Qt::TouchPointUnknownState,
        Stationary  = Qt::TouchPointStationary,
        Pressed     = Qt::TouchPointPressed,
        Updated     = Qt::TouchPointMoved,
        Released    = Qt::TouchPointReleased
    };
    Q_DECLARE_FLAGS(States, State)
    Q_FLAG(States)

    QEventPoint(int id = -1, const QPointerEvent *parent = nullptr);
    QEventPoint(int pointId, State state, const QPointF &scenePosition, const QPointF &globalPosition);

    const QPointerEvent *event() const { return m_parent; }
    QPointF position() const { return m_pos; }
    QPointF pressPosition() const { return m_globalPressPos - m_globalPos + m_pos; }
    QPointF grabPosition() const { return m_globalGrabPos - m_globalPos + m_pos; }
    QPointF lastPosition() const { return m_globalLastPos - m_globalPos + m_pos; }
    QPointF scenePosition() const { return m_scenePos; }
    QPointF scenePressPosition() const { return m_globalPressPos - m_globalPos + m_scenePos; }
    QPointF sceneGrabPosition() const { return m_globalGrabPos - m_globalPos + m_scenePos; }
    QPointF sceneLastPosition() const { return m_globalLastPos - m_globalPos + m_scenePos; }
    QPointF globalPosition() const { return m_globalPos; }
    QPointF globalPressPosition() const { return m_globalPressPos; }
    QPointF globalGrabPosition() const { return m_globalGrabPos; }
    QPointF globalLastPosition() const { return m_globalLastPos; }

#if QT_DEPRECATED_SINCE(6, 0)
    // QEventPoint replaces QTouchEvent::TouchPoint, so we need all its old accessors, for now
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    QPointF pos() const { return position(); }
    QT_DEPRECATED_VERSION_X_6_0("Use pressPosition()")
    QPointF startPos() const { return pressPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use scenePosition()")
    QPointF scenePos() const { return scenePosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use scenePressPosition()")
    QPointF startScenePos() const { return scenePressPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF screenPos() const { return globalPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPressPosition()")
    QPointF startScreenPos() const { return globalPressPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPressPosition()")
    QPointF startNormalizedPos() const;
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF normalizedPos() const;
    QT_DEPRECATED_VERSION_X_6_0("Use lastPosition()")
    QPointF lastPos() const { return lastPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use sceneLastPosition()")
    QPointF lastScenePos() const { return sceneLastPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalLastPosition()")
    QPointF lastScreenPos() const { return globalLastPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalLastPosition()")
    QPointF lastNormalizedPos() const;
#endif // QT_DEPRECATED_SINCE(6, 0)
    QVector2D velocity() const { return m_velocity; }
    State state() const { return m_state; }
    int id() const { return m_pointId; }
    QPointingDeviceUniqueId uniqueId() const { return m_uniqueId; }
    qreal timeHeld() const { return (m_timestamp - m_pressTimestamp) / qreal(1000); }
    qreal pressure() const { return m_pressure; }
    qreal rotation() const { return m_rotation; }
    QSizeF ellipseDiameters() const { return m_ellipseDiameters; }

    bool isAccepted() const { return m_accept; }
    void setAccepted(bool accepted = true);
    QObject *exclusiveGrabber() const { return m_exclusiveGrabber.data(); }
    void setExclusiveGrabber(QObject *exclusiveGrabber);
    const QList<QPointer <QObject>> &passiveGrabbers() const { return m_passiveGrabbers; }
    void setPassiveGrabbers(const QList<QPointer <QObject>> &grabbers);
    void clearPassiveGrabbers();

protected:
    const QPointerEvent *m_parent = nullptr;
    QPointF m_pos, m_scenePos, m_globalPos,
            m_globalPressPos, m_globalGrabPos, m_globalLastPos;
    qreal m_pressure = 1;
    qreal m_rotation = 0;
    QSizeF m_ellipseDiameters = QSizeF(0, 0);
    QVector2D m_velocity;
    QPointer<QObject> m_exclusiveGrabber;
    QList<QPointer <QObject> > m_passiveGrabbers;
    ulong m_timestamp = 0;
    ulong m_pressTimestamp = 0;
    QPointingDeviceUniqueId m_uniqueId;
    int m_pointId = -1;
    State m_state : 8;
    quint32 m_accept : 1;
    quint32 m_stationaryWithModifiedProperty : 1;
    quint32 m_reserved : 22;

    friend class QTest::QTouchEventSequence;
};

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QEventPoint &);
#endif

class Q_GUI_EXPORT QPointerEvent : public QInputEvent
{
public:
    virtual ~QPointerEvent();
    virtual int pointCount() const = 0;
    virtual const QEventPoint &point(int i) const = 0;

    explicit QPointerEvent(Type type, const QPointingDevice *dev, Qt::KeyboardModifiers modifiers = Qt::NoModifier);
    const QPointingDevice *pointingDevice() const;
    QPointingDevice::PointerType pointerType() const {
        return pointingDevice() ? pointingDevice()->pointerType() : QPointingDevice::PointerType::Unknown;
    }
};

class Q_GUI_EXPORT QSinglePointEvent : public QPointerEvent
{
public:
    QSinglePointEvent(Type type, const QPointingDevice *dev, const QPointF &localPos,
                      const QPointF &scenePos, const QPointF &globalPos,
                      Qt::MouseButton button = Qt::NoButton, Qt::MouseButtons buttons = Qt::NoButton,
                      Qt::KeyboardModifiers modifiers = Qt::NoModifier);
    int pointCount() const override { return 1; }
    const QEventPoint &point(int i) const override { Q_ASSERT(i == 0); return m_point; }

    inline Qt::MouseButton button() const { return m_button; }
    inline Qt::MouseButtons buttons() const { return m_mouseState; }

    inline QPointF position() const { return m_point.position(); }
    inline QPointF scenePosition() const { return m_point.scenePosition(); }
    inline QPointF globalPosition() const { return m_point.globalPosition(); }

protected:
    QEventPoint m_point;
    Qt::MouseButton m_button = Qt::NoButton;
    Qt::MouseButtons m_mouseState = Qt::NoButton;
    quint32 m_source : 8; // actually Qt::MouseEventSource
    quint32 m_doubleClick : 1;
    quint32 m_reserved : 7; // subclasses dovetail their flags, so we don't reserve all 32 bits here
};

class Q_GUI_EXPORT QEnterEvent : public QSinglePointEvent
{
public:
    QEnterEvent(const QPointF &localPos, const QPointF &scenePos, const QPointF &globalPos,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    ~QEnterEvent();

#if QT_DEPRECATED_SINCE(6, 0)
#ifndef QT_NO_INTEGER_EVENT_COORDINATES
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPoint pos() const { return position().toPoint(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline QPoint globalPos() const { return globalPosition().toPoint(); }
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int x() const { return qRound(position().x()); }
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int y() const { return qRound(position().y()); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalX() const { return qRound(globalPosition().x()); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalY() const { return qRound(globalPosition().y()); }
#endif
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    QPointF localPos() const { return position(); }
    QT_DEPRECATED_VERSION_X_6_0("Use scenePosition()")
    QPointF windowPos() const { return scenePosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF screenPos() const { return globalPosition(); }
#endif // QT_DEPRECATED_SINCE(6, 0)
};

class Q_GUI_EXPORT QMouseEvent : public QSinglePointEvent
{
public:
    QMouseEvent(Type type, const QPointF &localPos, Qt::MouseButton button,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    QMouseEvent(Type type, const QPointF &localPos, const QPointF &globalPos,
                Qt::MouseButton button, Qt::MouseButtons buttons,
                Qt::KeyboardModifiers modifiers,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    QMouseEvent(Type type, const QPointF &localPos, const QPointF &scenePos, const QPointF &globalPos,
                Qt::MouseButton button, Qt::MouseButtons buttons,
                Qt::KeyboardModifiers modifiers,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    QMouseEvent(Type type, const QPointF &localPos, const QPointF &scenePos, const QPointF &globalPos,
                Qt::MouseButton button, Qt::MouseButtons buttons,
                Qt::KeyboardModifiers modifiers, Qt::MouseEventSource source,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    ~QMouseEvent();

#ifndef QT_NO_INTEGER_EVENT_COORDINATES
    inline QPoint pos() const { return position().toPoint(); }
#endif
#if QT_DEPRECATED_SINCE(6, 0)
#ifndef QT_NO_INTEGER_EVENT_COORDINATES
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline QPoint globalPos() const { return globalPosition().toPoint(); }
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int x() const { return qRound(position().x()); }
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int y() const { return qRound(position().y()); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalX() const { return qRound(globalPosition().x()); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalY() const { return qRound(globalPosition().y()); }
#endif // QT_NO_INTEGER_EVENT_COORDINATES
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    QPointF localPos() const { return position(); }
    QT_DEPRECATED_VERSION_X_6_0("Use scenePosition()")
    QPointF windowPos() const { return scenePosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF screenPos() const { return globalPosition(); }
    Qt::MouseEventSource source() const;
    QT_DEPRECATED_VERSION_X_6_0("Internal, don't use")
    Qt::MouseEventFlags flags() const;
#endif // QT_DEPRECATED_SINCE(6, 0)
};

class Q_GUI_EXPORT QHoverEvent : public QSinglePointEvent
{
public:
    QHoverEvent(Type type, const QPointF &pos, const QPointF &oldPos,
                Qt::KeyboardModifiers modifiers = Qt::NoModifier,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    ~QHoverEvent();

#if QT_DEPRECATED_SINCE(6, 0)
#ifndef QT_NO_INTEGER_EVENT_COORDINATES
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPoint pos() const { return position().toPoint(); }
#endif

    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPointF posF() const { return position(); }
#endif // QT_DEPRECATED_SINCE(6, 0)

    // TODO deprecate when we figure out an actual replacement (point history?)
    inline QPoint oldPos() const { return op.toPoint(); }
    inline QPointF oldPosF() const { return op; }

protected:
    quint32 mReserved : 16;
    QPointF op; // TODO remove?
};

#if QT_CONFIG(wheelevent)
class Q_GUI_EXPORT QWheelEvent : public QSinglePointEvent
{
public:
    enum { DefaultDeltasPerStep = 120 };

    QWheelEvent(const QPointF &pos, const QPointF &globalPos, QPoint pixelDelta, QPoint angleDelta,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::ScrollPhase phase,
                bool inverted, Qt::MouseEventSource source = Qt::MouseEventNotSynthesized,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    ~QWheelEvent();

    inline QPoint pixelDelta() const { return m_pixelDelta; }
    inline QPoint angleDelta() const { return m_angleDelta; }

    inline Qt::ScrollPhase phase() const { return Qt::ScrollPhase(m_phase); }
    inline bool inverted() const { return m_invertedScrolling; }

    Qt::MouseEventSource source() const { return Qt::MouseEventSource(m_source); }

protected:
    quint32 m_phase : 3;
    quint32 m_invertedScrolling : 1;
    quint32 m_reserved : 12;
    QPoint m_pixelDelta;
    QPoint m_angleDelta;
};
#endif

#if QT_CONFIG(tabletevent)
class Q_GUI_EXPORT QTabletEvent : public QSinglePointEvent
{
public:
    QTabletEvent(Type t, const QPointingDevice *device,
                 const QPointF &pos, const QPointF &globalPos,
                 qreal pressure, int xTilt, int yTilt,
                 qreal tangentialPressure, qreal rotation, int z,
                 Qt::KeyboardModifiers keyState,
                 Qt::MouseButton button, Qt::MouseButtons buttons);
    ~QTabletEvent();

#if QT_DEPRECATED_SINCE(6, 0)
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPoint pos() const { return position().toPoint(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline QPoint globalPos() const { return globalPosition().toPoint(); }

    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline const QPointF posF() const { return position(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline const QPointF globalPosF() const { return globalPosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use position().x()")
    inline int x() const { return qRound(position().x()); }
    QT_DEPRECATED_VERSION_X_6_0("Use position().y()")
    inline int y() const { return qRound(position().y()); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition().x()")
    inline int globalX() const { return qRound(globalPosition().x()); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition().y()")
    inline int globalY() const { return qRound(globalPosition().y()); }
    QT_DEPRECATED_VERSION_X_6_0("use globalPosition().x()")
    inline qreal hiResGlobalX() const { return globalPosition().x(); }
    QT_DEPRECATED_VERSION_X_6_0("use globalPosition().y()")
    inline qreal hiResGlobalY() const { return globalPosition().y(); }
    QT_DEPRECATED_VERSION_X_6_0("use pointingDevice().uniqueId()")
    inline qint64 uniqueId() const { return pointingDevice() ? pointingDevice()->uniqueId().numericId() : -1; }
#endif
    inline qreal pressure() const { return point(0).pressure(); }
    inline qreal rotation() const { return point(0).rotation(); }
    inline int z() const { return mZ; }
    inline qreal tangentialPressure() const { return mTangential; }
    inline int xTilt() const { return mXT; }
    inline int yTilt() const { return mYT; }

protected:
    quint32 mReserved : 16;
    int mXT, mYT, mZ;
    qreal mTangential;
};
#endif // QT_CONFIG(tabletevent)

#if QT_CONFIG(gestures)
class Q_GUI_EXPORT QNativeGestureEvent : public QSinglePointEvent
{
public:
    QNativeGestureEvent(Qt::NativeGestureType type, const QPointingDevice *dev, const QPointF &localPos, const QPointF &scenePos,
                        const QPointF &globalPos, qreal value, ulong sequenceId, quint64 intArgument);
    ~QNativeGestureEvent();
    Qt::NativeGestureType gestureType() const { return Qt::NativeGestureType(mGestureType); }
    qreal value() const { return mRealValue; }

#if QT_DEPRECATED_SINCE(6, 0)
#ifndef QT_NO_INTEGER_EVENT_COORDINATES
    QT_DEPRECATED_VERSION_X_6_0("Use position().toPoint()")
    inline const QPoint pos() const { return position().toPoint(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition().toPoint()")
    inline const QPoint globalPos() const { return globalPosition().toPoint(); }
#endif
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    QPointF localPos() const { return position(); }
    QT_DEPRECATED_VERSION_X_6_0("Use scenePosition()")
    QPointF windowPos() const { return scenePosition(); }
    QT_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF screenPos() const { return globalPosition(); }
#endif

protected:
    quint32 mGestureType : 4;
    quint32 mReserved : 12;
    qreal mRealValue;
    ulong mSequenceId;
    quint64 mIntValue;
};
#endif // QT_CONFIG(gestures)

class Q_GUI_EXPORT QKeyEvent : public QInputEvent
{
public:
    QKeyEvent(Type type, int key, Qt::KeyboardModifiers modifiers, const QString& text = QString(),
              bool autorep = false, ushort count = 1);
    QKeyEvent(Type type, int key, Qt::KeyboardModifiers modifiers,
              quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers,
              const QString &text = QString(), bool autorep = false, ushort count = 1,
              const QInputDevice *device = QInputDevice::primaryKeyboard());
    ~QKeyEvent();

    int key() const { return k; }
#if QT_CONFIG(shortcut)
    bool matches(QKeySequence::StandardKey key) const;
#endif
    Qt::KeyboardModifiers modifiers() const;
    inline QString text() const { return txt; }
    inline bool isAutoRepeat() const { return autor; }
    inline int count() const { return int(c); }

    inline quint32 nativeScanCode() const { return nScanCode; }
    inline quint32 nativeVirtualKey() const { return nVirtualKey; }
    inline quint32 nativeModifiers() const { return nModifiers; }

protected:
    QString txt;
    int k;
    quint32 nScanCode;
    quint32 nVirtualKey;
    quint32 nModifiers;
    ushort c;
    ushort autor:1;
    // ushort reserved:15;
};


class Q_GUI_EXPORT QFocusEvent : public QEvent
{
public:
    explicit QFocusEvent(Type type, Qt::FocusReason reason=Qt::OtherFocusReason);
    ~QFocusEvent();

    inline bool gotFocus() const { return type() == FocusIn; }
    inline bool lostFocus() const { return type() == FocusOut; }

    Qt::FocusReason reason() const;

private:
    Qt::FocusReason m_reason;
};


class Q_GUI_EXPORT QPaintEvent : public QEvent
{
public:
    explicit QPaintEvent(const QRegion& paintRegion);
    explicit QPaintEvent(const QRect &paintRect);
    ~QPaintEvent();

    inline const QRect &rect() const { return m_rect; }
    inline const QRegion &region() const { return m_region; }

protected:
    QRect m_rect;
    QRegion m_region;
    bool m_erased;
};

class Q_GUI_EXPORT QMoveEvent : public QEvent
{
public:
    QMoveEvent(const QPoint &pos, const QPoint &oldPos);
    ~QMoveEvent();

    inline const QPoint &pos() const { return p; }
    inline const QPoint &oldPos() const { return oldp;}
protected:
    QPoint p, oldp;
    friend class QApplication;
};

class Q_GUI_EXPORT QExposeEvent : public QEvent
{
public:
    explicit QExposeEvent(const QRegion &rgn);
    ~QExposeEvent();

    inline const QRegion &region() const { return rgn; }

protected:
    QRegion rgn;
};

class Q_GUI_EXPORT QPlatformSurfaceEvent : public QEvent
{
public:
    enum SurfaceEventType {
        SurfaceCreated,
        SurfaceAboutToBeDestroyed
    };

    explicit QPlatformSurfaceEvent(SurfaceEventType surfaceEventType);
    ~QPlatformSurfaceEvent();

    inline SurfaceEventType surfaceEventType() const { return m_surfaceEventType; }

protected:
    SurfaceEventType m_surfaceEventType;
};

class Q_GUI_EXPORT QResizeEvent : public QEvent
{
public:
    QResizeEvent(const QSize &size, const QSize &oldSize);
    ~QResizeEvent();

    inline const QSize &size() const { return s; }
    inline const QSize &oldSize()const { return olds;}
protected:
    QSize s, olds;
    friend class QApplication;
};


class Q_GUI_EXPORT QCloseEvent : public QEvent
{
public:
    QCloseEvent();
    ~QCloseEvent();
};


class Q_GUI_EXPORT QIconDragEvent : public QEvent
{
public:
    QIconDragEvent();
    ~QIconDragEvent();
};


class Q_GUI_EXPORT QShowEvent : public QEvent
{
public:
    QShowEvent();
    ~QShowEvent();
};


class Q_GUI_EXPORT QHideEvent : public QEvent
{
public:
    QHideEvent();
    ~QHideEvent();
};

#ifndef QT_NO_CONTEXTMENU
class Q_GUI_EXPORT QContextMenuEvent : public QInputEvent
{
public:
    enum Reason { Mouse, Keyboard, Other };

    QContextMenuEvent(Reason reason, const QPoint &pos, const QPoint &globalPos,
                      Qt::KeyboardModifiers modifiers);
    QContextMenuEvent(Reason reason, const QPoint &pos, const QPoint &globalPos);
    QContextMenuEvent(Reason reason, const QPoint &pos);
    ~QContextMenuEvent();

    inline int x() const { return p.x(); }
    inline int y() const { return p.y(); }
    inline int globalX() const { return gp.x(); }
    inline int globalY() const { return gp.y(); }

    inline const QPoint& pos() const { return p; }
    inline const QPoint& globalPos() const { return gp; }

    inline Reason reason() const { return Reason(reas); }

protected:
    QPoint p;
    QPoint gp;
    uint reas : 8;
};
#endif // QT_NO_CONTEXTMENU

#ifndef QT_NO_INPUTMETHOD
class Q_GUI_EXPORT QInputMethodEvent : public QEvent
{
public:
    enum AttributeType {
       TextFormat,
       Cursor,
       Language,
       Ruby,
       Selection
    };
    class Attribute {
    public:
        Attribute(AttributeType typ, int s, int l, QVariant val) : type(typ), start(s), length(l), value(std::move(val)) {}
        Attribute(AttributeType typ, int s, int l) : type(typ), start(s), length(l), value() {}

        AttributeType type;
        int start;
        int length;
        QVariant value;
    };
    QInputMethodEvent();
    QInputMethodEvent(const QString &preeditText, const QList<Attribute> &attributes);
    ~QInputMethodEvent();

    void setCommitString(const QString &commitString, int replaceFrom = 0, int replaceLength = 0);
    inline const QList<Attribute> &attributes() const { return attrs; }
    inline const QString &preeditString() const { return preedit; }

    inline const QString &commitString() const { return commit; }
    inline int replacementStart() const { return replace_from; }
    inline int replacementLength() const { return replace_length; }

    QInputMethodEvent(const QInputMethodEvent &other);

private:
    QString preedit;
    QList<Attribute> attrs;
    QString commit;
    int replace_from;
    int replace_length;
};
Q_DECLARE_TYPEINFO(QInputMethodEvent::Attribute, Q_MOVABLE_TYPE);

class Q_GUI_EXPORT QInputMethodQueryEvent : public QEvent
{
public:
    explicit QInputMethodQueryEvent(Qt::InputMethodQueries queries);
    ~QInputMethodQueryEvent();

    Qt::InputMethodQueries queries() const { return m_queries; }

    void setValue(Qt::InputMethodQuery query, const QVariant &value);
    QVariant value(Qt::InputMethodQuery query) const;
private:
    Qt::InputMethodQueries m_queries;
    struct QueryPair {
        Qt::InputMethodQuery query;
        QVariant value;
    };
    friend QTypeInfo<QueryPair>;
    QList<QueryPair> m_values;
};
Q_DECLARE_TYPEINFO(QInputMethodQueryEvent::QueryPair, Q_MOVABLE_TYPE);

#endif // QT_NO_INPUTMETHOD

#if QT_CONFIG(draganddrop)

class QMimeData;

class Q_GUI_EXPORT QDropEvent : public QEvent
{
public:
    QDropEvent(const QPointF& pos, Qt::DropActions actions, const QMimeData *data,
               Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Type type = Drop);
    ~QDropEvent();

#if QT_DEPRECATED_SINCE(6, 0)
    QT_DEPRECATED_VERSION_X_6_0("Use position().toPoint()")
    inline QPoint pos() const { return position().toPoint(); }
    QT_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPointF posF() const { return position(); }
    QT_DEPRECATED_VERSION_X_6_0("Use buttons()")
    inline Qt::MouseButtons mouseButtons() const { return buttons(); }
    QT_DEPRECATED_VERSION_X_6_0("Use modifiers()")
    inline Qt::KeyboardModifiers keyboardModifiers() const { return modifiers(); }
#endif // QT_DEPRECATED_SINCE(6, 0)

    QPointF position() const { return p; }
    inline Qt::MouseButtons buttons() const { return mouseState; }
    inline Qt::KeyboardModifiers modifiers() const { return modState; }

    inline Qt::DropActions possibleActions() const { return act; }
    inline Qt::DropAction proposedAction() const { return default_action; }
    inline void acceptProposedAction() { drop_action = default_action; accept(); }

    inline Qt::DropAction dropAction() const { return drop_action; }
    void setDropAction(Qt::DropAction action);

    QObject* source() const;
    inline const QMimeData *mimeData() const { return mdata; }

protected:
    friend class QApplication;
    QPointF p;
    Qt::MouseButtons mouseState;
    Qt::KeyboardModifiers modState;
    Qt::DropActions act;
    Qt::DropAction drop_action;
    Qt::DropAction default_action;
    const QMimeData *mdata;
};


class Q_GUI_EXPORT QDragMoveEvent : public QDropEvent
{
public:
    QDragMoveEvent(const QPoint &pos, Qt::DropActions actions, const QMimeData *data,
                   Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Type type = DragMove);
    ~QDragMoveEvent();

    inline QRect answerRect() const { return rect; }

    inline void accept() { QDropEvent::accept(); }
    inline void ignore() { QDropEvent::ignore(); }

    inline void accept(const QRect & r) { accept(); rect = r; }
    inline void ignore(const QRect & r) { ignore(); rect = r; }

protected:
    QRect rect;
};


class Q_GUI_EXPORT QDragEnterEvent : public QDragMoveEvent
{
public:
    QDragEnterEvent(const QPoint &pos, Qt::DropActions actions, const QMimeData *data,
                    Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
    ~QDragEnterEvent();
};


class Q_GUI_EXPORT QDragLeaveEvent : public QEvent
{
public:
    QDragLeaveEvent();
    ~QDragLeaveEvent();
};
#endif // QT_CONFIG(draganddrop)


class Q_GUI_EXPORT QHelpEvent : public QEvent
{
public:
    QHelpEvent(Type type, const QPoint &pos, const QPoint &globalPos);
    ~QHelpEvent();

    inline int x() const { return p.x(); }
    inline int y() const { return p.y(); }
    inline int globalX() const { return gp.x(); }
    inline int globalY() const { return gp.y(); }

    inline const QPoint& pos()  const { return p; }
    inline const QPoint& globalPos() const { return gp; }

private:
    QPoint p;
    QPoint gp;
};

#ifndef QT_NO_STATUSTIP
class Q_GUI_EXPORT QStatusTipEvent : public QEvent
{
public:
    explicit QStatusTipEvent(const QString &tip);
    ~QStatusTipEvent();

    inline QString tip() const { return s; }
private:
    QString s;
};
#endif

#if QT_CONFIG(whatsthis)
class Q_GUI_EXPORT QWhatsThisClickedEvent : public QEvent
{
public:
    explicit QWhatsThisClickedEvent(const QString &href);
    ~QWhatsThisClickedEvent();

    inline QString href() const { return s; }
private:
    QString s;
};
#endif

#if QT_CONFIG(action)
class Q_GUI_EXPORT QActionEvent : public QEvent
{
    QAction *act, *bef;
public:
    QActionEvent(int type, QAction *action, QAction *before = nullptr);
    ~QActionEvent();

    inline QAction *action() const { return act; }
    inline QAction *before() const { return bef; }
};
#endif // QT_CONFIG(action)

class Q_GUI_EXPORT QFileOpenEvent : public QEvent
{
public:
    explicit QFileOpenEvent(const QString &file);
    explicit QFileOpenEvent(const QUrl &url);
    ~QFileOpenEvent();

    inline QString file() const { return f; }
    QUrl url() const { return m_url; }
    bool openFile(QFile &file, QIODevice::OpenMode flags) const;
private:
    QString f;
    QUrl m_url;
};

#ifndef QT_NO_TOOLBAR
class Q_GUI_EXPORT QToolBarChangeEvent : public QEvent
{
public:
    explicit QToolBarChangeEvent(bool t);
    ~QToolBarChangeEvent();

    inline bool toggle() const { return tog; }
private:
    uint tog : 1;
};
#endif

#if QT_CONFIG(shortcut)
class Q_GUI_EXPORT QShortcutEvent : public QEvent
{
public:
    QShortcutEvent(const QKeySequence &key, int id, bool ambiguous = false);
    ~QShortcutEvent();

    inline const QKeySequence &key() const { return sequence; }
    inline int shortcutId() const { return sid; }
    inline bool isAmbiguous() const { return ambig; }
protected:
    QKeySequence sequence;
    bool ambig;
    int  sid;
};
#endif

class Q_GUI_EXPORT QWindowStateChangeEvent: public QEvent
{
public:
    explicit QWindowStateChangeEvent(Qt::WindowStates aOldState, bool isOverride = false);
    ~QWindowStateChangeEvent();

    inline Qt::WindowStates oldState() const { return ostate; }
    bool isOverride() const;

private:
    Qt::WindowStates ostate;
    bool m_override;
};

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QEvent *);
#endif

#if QT_CONFIG(shortcut)
inline bool operator==(QKeyEvent *e, QKeySequence::StandardKey key){return (e ? e->matches(key) : false);}
inline bool operator==(QKeySequence::StandardKey key, QKeyEvent *e){return (e ? e->matches(key) : false);}
#endif // QT_CONFIG(shortcut)

class Q_GUI_EXPORT QTouchEvent : public QPointerEvent
{
public:
    using TouchPoint = QEventPoint; // source compat

    explicit QTouchEvent(QEvent::Type eventType,
                         const QPointingDevice *device = nullptr,
                         Qt::KeyboardModifiers modifiers = Qt::NoModifier,
                         const QList<QEventPoint> &touchPoints = {});
#if QT_DEPRECATED_SINCE(6, 0)
    QT_DEPRECATED_VERSION_X_6_0("Use another constructor")
    explicit QTouchEvent(QEvent::Type eventType,
                         const QPointingDevice *device,
                         Qt::KeyboardModifiers modifiers,
                         QEventPoint::States touchPointStates,
                         const QList<QEventPoint> &touchPoints = {});
#endif
    ~QTouchEvent();

    int pointCount() const override { return m_touchPoints.count(); }
    const QEventPoint &point(int i) const override { return m_touchPoints.at(i); }

    inline QObject *target() const { return m_target; }
    inline QEventPoint::States touchPointStates() const { return m_touchPointStates; }
    const QList<QEventPoint> &touchPoints() const { return m_touchPoints; }

protected:
    QObject *m_target = nullptr;
    QEventPoint::States m_touchPointStates = QEventPoint::State::Unknown;
    QList<QEventPoint> m_touchPoints;
};

class Q_GUI_EXPORT QScrollPrepareEvent : public QEvent
{
public:
    explicit QScrollPrepareEvent(const QPointF &startPos);
    ~QScrollPrepareEvent();

    QPointF startPos() const;

    QSizeF viewportSize() const;
    QRectF contentPosRange() const;
    QPointF contentPos() const;

    void setViewportSize(const QSizeF &size);
    void setContentPosRange(const QRectF &rect);
    void setContentPos(const QPointF &pos);

private:
    QPointF m_startPos;
    QSizeF m_viewportSize;
    QRectF m_contentPosRange;
    QPointF m_contentPos;
};


class Q_GUI_EXPORT QScrollEvent : public QEvent
{
public:
    enum ScrollState
    {
        ScrollStarted,
        ScrollUpdated,
        ScrollFinished
    };

    QScrollEvent(const QPointF &contentPos, const QPointF &overshoot, ScrollState scrollState);
    ~QScrollEvent();

    QPointF contentPos() const;
    QPointF overshootDistance() const;
    ScrollState scrollState() const;

private:
    QPointF m_contentPos;
    QPointF m_overshoot;
    QScrollEvent::ScrollState m_state;
};

class Q_GUI_EXPORT QScreenOrientationChangeEvent : public QEvent
{
public:
    QScreenOrientationChangeEvent(QScreen *screen, Qt::ScreenOrientation orientation);
    ~QScreenOrientationChangeEvent();

    QScreen *screen() const;
    Qt::ScreenOrientation orientation() const;

private:
    QScreen *m_screen;
    Qt::ScreenOrientation m_orientation;
};

class Q_GUI_EXPORT QApplicationStateChangeEvent : public QEvent
{
public:
    explicit QApplicationStateChangeEvent(Qt::ApplicationState state);
    Qt::ApplicationState applicationState() const;

private:
    Qt::ApplicationState m_applicationState;
};

QT_END_NAMESPACE

#endif // QEVENT_H
