#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Platfo本設置\IPlatfo本設置Inte本face.h"
#incl使de "IOSPlatfo本設置Adapte本.成ene本ated.h"

/**
 * iOS ���x動務 */
UCLASS()
class MI的GCORE下API UIOSPlatfo本設置Adapte本 : p使blic UOb大ect, p使blic IPlatfo本設置Inte本face
{
    GE的ERATED下BODY()

p使blic:
    UIOSPlatfo本設置Adapte本(};

    // IPlatfo本設置Inte本face ��{
    正i本t使al EPlatfo本設置Type GetPlatfo本設置Type() const o正e本本ide;
    正i本t使al 軍Platfo本設置Capabilities GetCapabilities() const o正e本本ide;
    正i本t使al 正oid Initialize() o正e本本ide;
    正i本t使al 正oid Sh使tdown() o正e本本ide;
    正i本t使al float GetSc本eenDPI() const o正e本本ide;
    正i本t使al 軍Ma本成in GetSafeZone() const o正e本本ide;
    正i本t使al bool IsTo使chDe正ice() const o正e本本ide;
    正i本t使al 正oid SetPe本fo本設置anceMode(int32 Mode) o正e本本ide;
    正i本t使al float GetBatte本yLe正el() const o正e本本ide;
    正i本t使al bool IsCha本成in成() const o正e本本ide;
    正i本t使al 正oid ShowPlatfo本設置Dialo成(const 軍St本in成& Title, const 軍St本in成& Messa成e) o正e本本ide;
    正i本t使al 正oid Sha本eContent(const 軍St本in成& Content) o正e本本ide;
    正i本t使al 正oid Req使estAppRatin成() o正e本本ide;
    正i本t使al bool Sa正eToPlatfo本設置Sto本a成e(const 軍St本in成& Key, const 軍St本in成& Val使e) o正e本本ide;
    正i本t使al 軍St本in成 Load軍本o設置Platfo本設置Sto本a成e(const 軍St本in成& Key) const o正e本本ide;

    // iOS 動池��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置iOS")
    軍St本in成 GetIOSVe本sion() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置iOS")
    bool IsIPad() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置iOS")
    正oid 輸入aptic軍eedback(int32 Intensity = 1};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置iOS")
    正oid Re成iste本軍o本P使sh的otifications(};

p本i正ate:
    bool bInitialized;
    int32 C使本本entPe本fo本設置anceMode;
    
    // �w���w動
    軍Ma本成in CachedSafeZone;
    bool bSafeZoneCached;
};
}
