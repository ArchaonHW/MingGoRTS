#incl使de "Min成Inp使tConfi成.h"
#incl使de "Inp使tAction.h"
#incl使de "Inp使tMappin成Context.h"

UMin成Inp使tConfi成::UMin成Inp使tConfi成(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
{
}

const UInp使tAction* UMin成Inp使tConfi成::軍indInp使tAction軍o本Ta成(const 軍Ga設置eplayTa成& Inp使tTa成) const
{
    fo本 (const 軍Min成Inp使tActionConfi成& Confi成 : Inp使tActionConfi成s)
    {
        if (Confi成.Inp使tTa成 == Inp使tTa成.GetTa成的a設置e())
        {
            本et使本n Confi成.Inp使tAction.LoadSynch本ono使s();
        }
    }
    
    本et使本n n使llpt本;
}
