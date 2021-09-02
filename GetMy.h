#ifndef GETMY_H
#define GETMY_H
#include <QStackedWidget>

class QcmExercice;
class FntSetting;
class GetMy
{
public:
    static GetMy & GetInstance()
    {
        static GetMy instance;
        return instance;
    }

    void SetQcmExerciceWidget(QcmExercice* exercice) { qcmExerciceInstance = exercice;}
    QcmExercice& QcmExerciceWidget() { return *qcmExerciceInstance; }

    void SetFntSettingWidget(FntSetting* fntSetting) { fntSettingInstance = fntSetting;}
    FntSetting& FntSettingWidget() { return *fntSettingInstance; }

private :
    GetMy() = default;
    QcmExercice* qcmExerciceInstance;
    FntSetting* fntSettingInstance;
};


#endif // GETMY_H
