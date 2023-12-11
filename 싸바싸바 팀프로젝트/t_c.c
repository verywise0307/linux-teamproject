#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#define MAXLINE 256
#define MAXSHM 1024

int chat_shmid;
char *chatlog;
int bag_shmid;
char *bag;
int progress_shmid;
char *progress;

int pi; //인게임 화면 상태 저장
int ps; //스크립트 저장

int starttime;
int finishtime;
char playtime[20];

char psw[10][MAXLINE] = {"\0", "\0", "134\n", "01064030572\n", "", "복실이\n", "오른쪽 왼쪽 왼쪽 오른쪽 오른쪽 왼쪽\n", "초록 보라 보라 노랑 빨강 파랑 주황 파랑 노랑 주황\n", "", ""};

int item[2];

int usingitem[5];
int itemfull = 0;
int usingitemfull = 0;

char hpipe[20] = "./host";
char gpipe[20] = "./guest";
char cpipe[20] = "./chat";
char cpipe0[20] = "./chat0\n";

//아이템 사용 여부
int glove = 0;
int chopstick = 0;
int steak = 0;
int cupboard = 0;

//아이템 획득 여부
int getglove = 0;
int getchopstick = 0;
int getsteak = 0;
int getcupboard = 0;

void intHandlerh(){
	unlink(hpipe);
	unlink(gpipe);
	unlink(cpipe);
	shmctl(progress_shmid, IPC_RMID, NULL);
	exit(1);
}

 void intHandlerg(){
    unlink(cpipe0);
    exit(1);
}

void intro()
{
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                           .irLLLr:.          i2gQBBBBBQQdI7.       iLXbggDXL.       .irr::                                           \n");
	printf("                                       rMBQBBgPdbgBBBBBBPL:bBBBEvi.....:rJqQBBQR5BQBBDILrr7XRBBB1PBBBBgDEQBBBBg.                                      \n");
	printf("                           iXQBBBBBBBgBBb.            .iUEBQi                  7gI             LBMi          .BBBQBBBBBBgs.                           \n");
	printf("                        :BBBPv.                                                                                       .iuQBBg.                        \n");
	printf("                       gBP                   gBP         rJ      EU          SBQBBBBBBBBBB         gZBBBBBBBBBBBK            BBJ                      \n");
	printf("                       vBE          i1:      BBB       .BBBB     BB.         BBB                   gBBI      .BQB            QB:                      \n");
	printf("                       iBQB.     .BBBBBQ     BBB      rBBEBBB    QBQgBBB:    BB2                   BQBi      iBBB          iQBQ.                      \n");
	printf("                    :BBR:       :BBg  BBB    BBB     sBBS  BBBr  BBB         BBBBBBBBBBBBB         qBBBBBBBBBBBBg             vBBM                    \n");
	printf("                   7BB          BBB   sBB.   dBB    iBBP    PQR  gBB               BB                    bBB                      BB.                 \n");
	printf("                   .BBY.        BB1   iBB:   SQB                                   BB                    bBZ                  .JBB                    \n");
	printf("                     .KdBBL     BBB   MQB    sBB           :UPgE2.           BBBBgbPQQBBBBB        BBBBSUsPbgQBQBi         PBBZX:      ..             \n");
	printf("              7512K    UBX      .BBBKBBBr    LBB        iBQBBBBBBBBBI                                                      BB:      BE77JQ.           \n");
	printf("             QB   BB   BB         UBBBJ      LBB        BBB       JBBj       :BBBQBBBBBBBB7         UBBQBBBQBBBE            rBB      BK  iBr          \n");
	printf("              JPuSJ     QBQZSk               kBB        iBBBDVIbgBBBB        BBBU      PBBB        iBBB      BBBr       jkPBBR        rjJJ            \n");
	printf("          i                isJ                            ibBBBBBDj           ZBBQBBBQBBBI          UBBBBQBBBBBU        dVr                   ii      \n");
	printf("        QPiBB                                                                                                               iii             XBivBL    \n");
	printf("        BUvXv        vPBBBBBBBj          iBv                                                                             PBQBBBBBBBVri       XvvQi    \n");
	printf("                rBQBBBBBBBBBBBQB          PBBRr          irZgi          gBBXi          LBBBv             sBBi          iBBBBQBBBBBBBQBQBui            \n");
	printf("           iLZBQBBBBBBBBBQBBBBBQBQ          iXBBBBB BBBBBBR  BBBBMQQBBBBBi ZBBBBBBBBBBBgL iQBBBBBBBQBBBQBQs           BQBBBBBQBBBBBBBBBBBBBBP         \n");
	printf("           SQBBBBBQBBBBBQBBBBBBBBBB                ii           iiii                              iiii               BBBBBBBBBQBBBBBBBBBBBBBg         \n");
	printf("           ZBBBBBQBBBBBBBBBBBBBBBBBS                                                                                iBBBBBBBBBBBBBBBBQBBBQBBB         \n");
	printf("           BBBBBBBBBBBBBBBQBBBBBBBQB                                                                                uBBBBBQBBBQBQBBBBBBBBBQBB         \n");
	printf("           iIBBBBBBBBQBBBBBBBBBQBBBB                                                                                PQBBBBBBBBBQBBBBBBBQBBB           \n");
	printf("              BBBBQBBBBBBBQBBBBBBBBB                     rvvruIU   huSXEr    iD      ki  i                          uBBBBBQBBBQBBBBBQBBBQB            \n");
	printf("              jBBQBBBBBQBBBBBQBBBQBQ                     IVXBJvs   BB        B B     BB  BB                         iBBBBQBBBBBBBBBBBBBQBg            \n");
	printf("              PBQBQBQBBBBBBBBBBBBBBK                        B      BBuXZI   BBrBV   KBQuBPB                          BBBBBBBBBBBBBBBBBBBBB            \n");
	printf("             iBQBBBQBBBBBBBQBQBQBQBB                        Bi     BP      vB  LB   Bi BB BB                        DBBBBBBBBQBQBBBBBBBBBBP           \n");
	printf("              ivBBBBBBBBBBBBBBBBBBBBg                       Qr     RBgRBg  ZB   Bi  B  B   B                      iBQBBBBBQBBBBBBBBBBBBB              \n");
	printf("                 iBBBBBQBBBBBBBBBBBBBBBui                                                                     rVBBBBBBBBBBBBBBBBBBBBBB                \n");
	printf("                   BBBBBBBBBBBBBBBBBBBQBQBUi                                                                 iIBBBBBQBQBBBBBBBBQQBBBBi                \n");
	printf("                    svi   SBBBBBBBBBBBDi                                                                          qBBQBBBBBBEi                        \n");
	printf("                             BBBQBBBI         i      Bi       M   is  Bi        u   i  Bk       Qv   gr Bd          XBQBQBQ                           \n");
	printf("                              BBBBg           B  Bi  Qi       Bi  BB  Br        BK BZ  BB       BB   Br BB            BBBB                            \n");
	printf("                              rBBr           UQs Bi  Bq       QBddQB  BBDBi     BB Bg  BBJU     BBgQBBv BBMBI          KBB                            \n");
	printf("                              iB             BBBBBB  BBPR     Bq  VB  BQ       DB BB P gB       BB   BU BQ              QB                            \n");
	printf("                              rK            iB UB SB BE       BBjuBB  BB       Bv BS B PB       QBZEQBU BB               J                            \n");
	printf("                                            ii     i Is       iivrii  gk               iV       iiiiiii iS                                            \n");
	printf("                                                                                                                                                      \n");
}

void art_3F()
{
	printf("         dB                           vB                              EB              Bk    iB   iKQBQVi       isZBBVQB                      BR   Bv  \n");
	printf("         PB                           iB                              bB              Qu     BuuLr        ivJIUYi    gQ                      BZ   Qi  \n");
	printf("         dB   BBiBd                   vB                              BQ              Bj            vuPbdIsi         RB  BbBr                BB   Bi  \n");
	printf("  jgIq   EQ   Bg BB                   vB                              BB              BY     iYqbdIYi           vdd  QQ  BqBv            iisuBL   Qr  \n");
	printf("  B  BB  dB    vs                     vB                              BQ   [ 찬장 ]   Bv     QE                 BPBv QB            iYIddPUvi      Bv  \n");
	printf("  RDjBi  ZB                           vB                              BM              Br     Bv                  i   QQ     ivuPdbkvi        isqPqZ   \n");
	printf("         dB                           vB                              Bb              Bi     QY                      QBvXPbIYi        rsKPEXuvi       \n");
	printf("         EB                           vB                              BX              Bi     Bv               irYXqPUvi        iYXPdKUvi              \n");
	printf("         dB          [ 문 ]           vB                              Bg              Bi     BY         iLXbEqur        iYKPEKUvi                   iv\n");
	printf("         EQ                           rB                       ivkPDPSvVMdi           Q      Bs  ivIqZqkvi       iLKPEXuvi                   ivSgBBBBB\n");
	printf("         EB                           vB                iruKDEdUvi       iKQgL        B      QQIIvi       iLKPdSuvi                   iuZIQBBBBQEjri  \n");
	printf("         EB                           rB         iisSEZdILi                  uRQUi    B            iYKPdXuri                          BBdBuii         \n");
	printf("         bB                           rB   iLkdEZXJii                           vDQKi B    ivJKPdXuri                                 Bv B            \n");
	printf("         ZB                          iZBXbKuri                                     iIQBuUqSuri                                        Qg Bi           \n");
	printf("         bB                   ivVPgdqLi                                                                 irsi                          BB QL           \n");
	printf("         EB            ivUqDEPuvi                                                                ivVZBQBBBBBB                         gB BI           \n");
	printf("         bB     iruXZZdUvi                                                                iikZBBBBBBZuri   QB                         KB Qg           \n");
	printf("         gBrkEZdIYi                                                                    iQBBBgduri          BB                         LB BB           \n");
	printf("   ivkbbPYrii                                                                          iB VB               KB                         iB gB           \n");
	printf("DEqUri                                                                                  B rB               jB                         iB kQ      iiJSE\n");
	printf("                                                                                        B iQ               rB                          B YB iJdZDKuri \n");
	printf("                                                        iruEQBBBBL                      Br B               iB                          BLrQBXi        \n");
	printf("                                                 irjdQBBBBBgSvi BB                      BJ Q                Bi                         Bdvi rJUr      \n");
	printf("                                             QBBQBBBDXvi        BB                      Bd Bi               Br                         Bi kBuLvYuXv   \n");
	printf("                                            uB QB               BQ                      BB Bv               BM                         Bv  B  vKKYBBBB\n");
	printf("                                            iB KB               dB                      MB BJ        iisSDdPkSQQY                      BX rBDqDBBgLi  \n");
	printf("                                             B vB               IB                      XB BX  iYVZggquri      iVQBSi              ivXgBBBMsvi       i\n");
	printf("                                             B iB               vB                      XBiBBMLii                  rPgUi    ivXQBBBBgUi             QB\n");
	printf("                                             Bi B               iB                      ZBjiivYi                      qBBQBBBgVr                   BB \n");
	printf("                                             Bv B                B                      UB vdJvYuPVr           ijDBBBBXvi                          BB \n");
	printf("                                             BK Bi               Br                     vB  QBijuvivSPs iYZQBBBQPvi                                iBB\n");
	printf("                                             BM Bi           ivuKBQbr                   iB  IB   XBBdBBBBEsi         irUKEZggRggZEVvi                P\n");
	printf("                                             QB Br    ivIbgDdIvi   iPBQj                 BriBBBBBEUvi           iSBBBgZKVuuYsLYYJJUIQBBr              \n");
	printf("                                             DB BBrjVKjvi              JgBSi        rIgBBBBEv                 DBBurriiii        iirv  rBB             \n");
	printf("                                             BBvvqgv                      iPBQUZBQBBQXvi                    EQQ  vvi                DX  BB            \n");
	printf("                                             gBvLiivuqji              iLXMMQBBXLi                          BB  iB                   LQ  QB            \n");
	printf("                                             KB  BBYkLLYXUi    iLqBBBBBPsi                [ 테이블 ]        QB  MB                 ivr rBB             \n");
	printf("                                             JQ  BB  ijdjSBBBBBBgki                                        PBq  rVri           iiiirIBBv              \n");
	printf("                                             iB  QBrVbQQBXvi            ivXDBBBQBBBBBRdvi                   iQBQriiiriiiiiiivvUXDQBBK                 \n");
	printf("                                           iiKBdVkri                vgQRqsi           ijDBRi                    vIIVkSIXVKSXIkLvi                     \n");
}

void art_B3F_1()
{
	printf("jvJYuJjJJLssujUjuYsYJYssjsjJuYsYJYuJujuYJYsYssjsusJsJYsYujujjvsYJLjsuJuYsYsLJJuuuujvusJYJYjsjsJLJvuuuusvjYsYjJjsjYjYjvsuujkvLsJYuJjYJsvvUuujkvvJjJuJuY\n");
	printf("QRQQQQRQRQqsvsLsvsIRQQQQQQQQRQQQZuvYLYvJIQQQRQQQQQQQQQSYvYLvLkgQQQQQQQQQRQQZssvsLLvqgQQBQQQQQQQQguLYLLvIgQQQQQRQQQQQgVLLLYvYqQQQQQQQQQqJvYvsvjVRRQMRMg\n");
	printf(" iiiiiiii vBLsuJLBQ iiiiiiiiiii iBSLuJLBb iiiiiiiiiii uBLJuvQQ iiiiiiiiiiiiiBvuJuvBI iiiiiiiiiiiiBVYuvQB iiiiiiiiiii BRvuJLBY iiiiiii vBLsuJvBg iii i \n");
	printf("           BvJJjvBU              BVYJjvBr             iBvusLRQ              BvJuJvBi             BUJsvBD             qQLJuvBi         iBvujuvBv       \n");
	printf("          iBvsusvQP              BSLusvBv             iQvsjvQB              BvuJJvBr             BbvjvQM             dQvusvBi         iBvJuJvBu       \n");
	printf("          iBvjJjvBq              BIsJjvBv             iBvjsLQB              BvJuYvQr             BPLsvbQ             ZQvJjvBi         iBvJjjvBj       \n");
	printf("          iBvYusvBP              BSLusvQL             rBvsjvQB              BvujjvBr             BbvuvDR             EBvusvBi         iBvYusvBu       \n");
	printf("          iBvJJjvBK              BVsJjvBv             iBvJJLRB              BvsuYvBr             BPvJvDQ             DQvsJvBi         iBvJsuvBu       \n");
	printf("          iBLsusvBP              QSLusvBL             rBvsjvQB              BvuJJvBr             BPvuvgQ             EBvjYvQi         iBvsuJvBU       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBLsusvBP              BSvusvBL             rBvYjvQB              BvJJJvBr             QPvuvgQ             EBvJsvBi         iBvsuJvBu       \n");
	printf("          iBvJJuvBK              BVYJuvBv             rBvjsLRB              BvsusvBr             BXssLMQ             DBvsjvBi         iBvJjjvBu       \n");
	printf("          iBLsusvBP              QVvuJvBY             rBvsjvQB              BvJJJvBr             BKvuvQR             EBvjsvQi         iQvsuJvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjJuvBq              BIssjvBv             rBvjsLQB              BvsusvBr             BKLJvgQ             DBLsjvBi         iBvjJuvBu       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("          iBvjsjvBS              BUssjvBr             iBvjJLBB              BjYjsvBr             BPLsvQQ             gBvsjvBi         iBvJJuvBs       \n");
	printf("JYkukukuUYbgYYJsYPQvuuUuUUkukuuYkBJYjsvdRvJjuJusuJuJjvgBvLjvKBvYuJususuJuJskBYsYJvDgPdDEDZgEDZDdPMuLJLIDgMQQQMQMQRQMMgVLJYLPgPZEDEgEZPDbYYuYLPQVPqSVXk\n");
	printf("BBBBBBBBQBDPEDEDdbdBBBBBBBBBQBBBRbdZEDEbZBBBBBBBBBBBQBgbEgEZbZBBBBBBQBBBBBBQPZZDZZPZgQRQQQRQMQMRgbbDdDbbZgDggMggDMDgEbPZEZEPEMMRgRRRMMdPdZEZdbbQRQRQQM\n");
	printf("vrvvvrvvvrvrvvvrvrvrvvvvvrvrvvvvvrvvvrvrvrvvvvvvvvvvvvvrvrvrvrvvvvvvvvvvvvvvvrvvvvvrvrvrvvvrvvvrvrvrvrvrvrvvvvvvvrvrvvvvvvvrvvvvvrvvvrvvvrvvvvvvvrrrrr\n");
	printf("                                                                                                                                                      \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiii iiiii i i i i iiiii iii iii iiiii iiiii iii i iii iiiiiii iii i iiiii iiiiiiiiiii i iii iii i iii iiiiiiiii iiiiiii iiiiiii i i i iii i iiii \n");
}

void art_B3F_2()
{
	printf("                         B                                    B                                       iBBi                   vB                       \n");
	printf("                         B                                    B                                     ivi                      iB                       \n");
	printf("                         Q                                    B                                  vBQ                         uB                       \n");
	printf("                         B                                    B                                  BUiiiiiiiiiiiiiiiiiiiiiiiiiiIB                       \n");
	printf("                         B                                    B                                  Bi                          iB                       \n");
	printf("                         B                                    B                                  BJ                          JB                       \n");
	printf("                         B                                    B                               iLjiiiiiiiiiiiiiiiiiiiiiiiiiiiiuB                       \n");
	printf("                         B                                    B                             Jgji                             rQ                       \n");
	printf("                         Q                                    B                           iQuiirvvvvvvvvvvvvvvvvLvLvvvvvvvvriPB                       \n");
	printf("                         B                                    B                           iB                                 iB                       \n");
	printf("                         Q                                    B                           iB                                 iB                       \n");
	printf("                         B                                    Q                           rBvvvLvYvLvLvLvLvLvYvLvLvYvLvLvYvviDB                       \n");
	printf("                         Q                                    B                         isu                                  iB                       \n");
	printf("                         B                                    B                       IBui                                   vB                       \n");
	printf("                         B                                    B                     SBviiirrvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvriIB                       \n");
	printf("                         B                                    B                     dS                                       iB                       \n");
	printf("                         B                                    B                     SV                                       iB                       \n");
	printf("                         B                                    B                     XBirrvrvrrrrrrrvrvrrrvrrrrrrrrrrrrrrrrrriuQ                       \n");
	printf("                         B             [ 콘센트 ]              B                  ijVr                                        rB                       \n");
	printf("                         B                                    Q                ruv                [ 계단 ]                    iQ                       \n");
	printf("                         Q           BUiriiiiiiiBi            B             iQBYi                                            rB                       \n");
	printf("                         B           B          Bi            B             BriiirrrrrrrrrrvrrrvrrrvrvrvrvrvrvrvvvvvrvrvvvrriUB                       \n");
	printf("                         B           B   B  B   Bi            B             B                                                iB                       \n");
	printf("                         B           B   i  i   Bi            B             B                                                iQ                       \n");
	printf("                         B           B          Bi            B             B                                                iB                       \n");
	printf("JJjuuuJuJuJuJujuJuuujusvvB           B   B  B   Qi            B            uBYuujuJuJuJuJuJusjJuJususjYJJuJuJusjJuJuJuuuJuJjvEB                       \n");
	printf("                         B           B   i  v   Bi            B         vSVi                                                 rB                       \n");
	printf("                         B           B          Bi            Q      ikqvi     i i i i i i i i i i i   i i i i i i i   i i   rB                       \n");
	printf("i i i i i i i i i i i    B           BjvvrvvivvvBi            B   ivqui     i i i i i i i i i i i i i i i i i i i i i i i i  rB                       \n");
	printf("i ii i i i i i i i i i   B                                    BLvuvi     i i i i i i i i i i i i i i i i i i i i i i i i i   iB                       \n");
	printf("i i i i i i i i i i i i  LsjUuIukUkuIUkUkUIUIUIukUkUkUIuIUIUuvvii   i i i i i i i i i i i i i i i i i i i i i i i i i i i i   vvYsJsuJjJjJjsjsJJuJjJus\n");
	printf("   i i i i i i i i i i i                                           i i i i i i i i i i i i              i i i i i i i i i i i                         \n");
	printf("i i i i i i i i i i i i i                                       i i i i i i i i i i i i i   [ 바닥 ]     i i i i i i i i i i i i i i i i   i   i    i \n");
	printf("   i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i             i i i i i i i i i i i i i i i i i i i i i i i  \n");
	printf("i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i \n");
	printf("i  i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i ii \n");
	printf("i iii i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i \n");
	printf("  ii iii i i i iii i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i iii i i i i i i iii i i i iii iiiii iii i i iii i i i i i i i i ii \n");
	printf("i iii i iiiii i iii iii i i iii iii iii iii i i i i i iiiii iii i iiiii i iiiiiii i i i iii i i i iiiii i i i i i iii iii i i i i i iii i i i i iiiii \n");
	printf("                                                                                                                                                      \n");
}

void art_2F()
{
	printf("          BX                                                                            rBi                   vB                                      \n");
	printf("        ri                                    [ 책장 ]                                    iv                  rB                                      \n");
	printf("     vPJi                                                                                   LPU               LB                                      \n");
	printf("    YBi iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii iuB              vB                                      \n");
	printf("    uv                                                                                         Q              LQ                                      \n");
	printf("    Jj  VJ BQvisBgvvvvrPBIvYvvvQBIvvBviiiiiiiiiiiiiiiBvLBBXvrvvvrvBBviibBuirrvvvdBvrrvvBY Q    B              vB                                      \n");
	printf("    Jj  kuYL  vQr iiiidqi iiirdYiii B                B iiiKdvivrvrrSBsiiLBdirrvrikBvirrrMBB    B              vB                                      \n");
	printf("    Lu  LBjvvMBvrsLYsBSrvsLvBPi iii B                B iii rQiivrriiiBjiiiBgirrriirBirrriLB    B              vB                                      \n");
	printf("    Yj  jS   qV iii iB  iii Br iiii B                BiiiiiiB iiiii  Br   bk iiii  B  ii iB    B              vB                                      \n");
	printf("    vu  jE i XZ iiiiiB iiii Qv iiiiiB                B iiiiiB iiiiii Br i Pq iiiiiiBiiiiiiQ    B              vB                                      \n");
	printf("    vj  ub i JR iiiiiBiiiii Bviiiii B                B iiiiiB iiiiii Bv i dq iiiiiiBiiiiiiB    Q              vB              [ 문 ]                  \n");
	printf("    ru  JE i YR iiii Biiiii QY iiiiiB               iB iiiiiB iiiiii Bv i qd iiiii BiiiiiiB    B              vB                                      \n");
	printf("    vJ  ud iivB iiii Biiiii BviiiiiESiiiiiiiiiiiiiiiiKbiiiiiB iiiiii Bs i Kb iiiii BiiiiiiB    Q              LB                                      \n");
	printf("    vu  jE i vB iiii Biiiii QY iisQi                  vBii iB iiiiii BY i kZ iiiii BiiiiiiQ    B              vB                                      \n");
	printf("    LJ  ub iiiB iiii Biiiii QL iMI                      BL iB iiiiii Ru i UE iiiii Bi iiiiB    B              vB                                      \n");
	printf("    vu  ud i iB iiii Br iii MviD                         kuiB iiiiii Rj i sR iiiii Br iiiiB    B              vB        ii                            \n");
	printf("    Jj  SBvvvYBrvvLvrBPrvvviBQDiiiiiiiiiiiiiiiiiiiiiiiiiiiuBBivvvvLvrBQrvrMBrvLvLvrBPrLvvuB    B              vB     iBsrirBi                         \n");
	printf("    su                                                                                         B              vB     BQ    BY                         \n");
	printf("    uJ                                                                                         B              LB      LJuuu                           \n");
	printf("    uj  iQ QPrrriEBLirrrivgBviiSBKrvrvriLBBVrVBiiiiiiiiiiiiiiiQBrvbBbrivrvdBJirvrvBBrivBs Qi   B              vB                                      \n");
	printf("    ss   BBjrvvrQBvrvvvrsBdrrrBBvrvvvrvSEvi irB               uS iiiuPriiiiiQSiiiiisBi iPYB    Q              LB                                      \n");
	printf("    vJ   BiiiiiuB iiiriiBriiiBS iiiriiYB  iiivM               YP iii iBKvvvvigBvvLvrvBsviUB    B              vB                                      \n");
	printf("    Ys   B iii vR iiiii Bi i Mv iiiii iQ iiiirQ               sd iiii Qv iii iB iii  Qi   B    B              vQ                                      \n");
	printf("    vj   B iii jQ iiiii Bi i Bv iiiiiiiB iiiirB               vD iiii Qv iii vQ iiii Biii B    B              vB                                      \n");
	printf("    sY   B iii UQ iiiii Biii BL iiiii rB iiiiiB               vg iiii BviiiiiYB iiiiiQiii B    Q              vB                                      \n");
	printf("    Yj   B iii SZ iiiii Qiii BviiiiiiiiB iii rB               VB iiii Qv iii UD iiiiiB iiiB    Q              vB                                      \n");
	printf("    Js   B iii Xb iiiii Biii Bv iiiiiiiQ ii vBiiiiiiiiiiiiiiiiiPqiiii Bviiii Vd iiiiiBiii B    Q              LB                                      \n");
	printf("    Yj   B iii dX iiiiiiBiii BriiiiiiiiB i KR                   rBrii Qv iii bK iiiiiB iiiB    B              vB                                      \n");
	printf("    js   B iii dk iiiii Biii Br iiiiiiiB iBJ                      dPi Briiii ZS iii iB ii B    B              LB                                      \n");
	printf("    Jj   B iii RJ iiiiiiBi i Bi iiiiiiiBig                         iPiBr iii Qs iiiiiB ii B    B              vB                                      \n");
	printf("    us   BvvLvrBgrvvLvvvBvvvrQVrvvvvvrsQBiiiiiiiiiiiiiiiiiiiiiiiiiiiiBBuvvvvrQErLvLvqBrvvvB    B              vB                                      \n");
	printf("    jU                                                                                         B              vB                                      \n");
	printf("    YI   vi usiiiILiriiiiPriiiiivPriiIiiiiiiiiiiiiiiivUirqviiiririvViiiiUviiiiiiiuviiiirI ir   B              vB                                      \n");
	printf("rriiqu   B rXi iEUiiii LZviiiiiPDviiiB               rBiiLZRviiiiiiuQKiirRgiiiiiivBViiiiIKUg   B              uB                                      \n");
	printf("iiiiKs   QBdrrXBIrvvvvBBvrvvvXbriiiiiB               iB iiiiQBvYjsYvvZBvvrXBUvJsjvvBBvsLvUBV   QvukkIkVUIkIUksXbLJujuuuuUuUuuuUuUuUuUuuuUuUuUuUuUuuuuj\n");
	printf("    rU   BviiiBr iiiiVB iiiiiBi iiiiiB               iQ iii vB iiiii  Qi   Bi iiii iB ii  vq   Q                                                      \n");
	printf("    vU   Bv i Br iii rB iiii BiiiiiiiB               iB iii JR iiiiii Biii Br iiiiirB iii uq   B                                                      \n");
	printf("    vU   Bvii QY iii vB iiii BiiiiiiiB               rQ iii jQ iiiiii Biii BriiiiiirB iiiiuP   B                                                      \n");
	printf("    vu   Bvii RjiiiiirB iiiiiBiiiiiiiB               vQ iii kR iiiiii Biii Bv iiiiiiB iiiiUK   B                                                      \n");
	printf("    rs   Qr i Pj iii iQ iiii BiiiiiivM iiiiiiiiiiiii vRiiii Yg iiiii  Bi i Br iiii iB iii YI   B                                                      \n");
}

void art_B2F()
{
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii EB iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii Bk iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii EB iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii Bk iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii dB iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii BU iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii EB iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii  Bk iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii dB iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii Bu iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii EB  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii  Bu iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii PB iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii  Bu iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii bB  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii  Qu iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii qB iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii BJ iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii qBuKbPdPbPPPbPbPPqbPbqbPPPPqPqPqPqPqPqPKPKPqbqPqPqPPbPPPbPbqPPdPbPbPdPdXUBs iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii PB                                                                       BJ iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii ZB                                                                       BU iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii dB                                                                       BU iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii DB  iii iiiiiivir                                          iiiiiiiiirr.  Bk iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii EB  DBBBBBBBBBQi                                           vBBBBBBBBBBv  Bk iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii gB   BBBBBBBBBBBBgv                                     rqBBBQBBBBBBBQ   QI iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii DB   DBBBBBBBBQBBBBBBi                               iQBBBBBBBBBBBBBBY   Bk iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii gB    BBBU    DBBQBBBBB                             BBBBBBBBE    QBBB    BV iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii DB    EBi      BBBQBBBBBv                         JBQBBBBBQd      qBY    Bk iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii gB              BBBBBQBBBu                       PBQBQBBBBB              BV iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii  BB              QBQBBBBBBBr                     UBBBQBBBBBB              BZ  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii  rBB               BBBBBBBBBBB                    iBBBBBBBBBBB              iBQr  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiiiii  iBQi               iBBQBBBBBBBBB                   BQBBBBBBBBBB                rBBr  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiiiii  iQBr                 QBBBBBBBBBQBBU                 DBBBBQBBBBBBBv                 rBBr  iiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiiii  iQBv                   BBBBBBBBBQBBBB                rBBBBQBBBQBBBBB                   iBBr  iiiiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiiiiii iMBL                    dBBBBBBBBBBQBBBB               BBQBBBBBBBBBBBBi                    iBBv  iiiiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiiii  igBU                     iQBBBBBQBBBQBBBBBd             BBBBQBQBBBBBBBBBQ                      iBBv  iiiiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiiiii iPBV                      JBBBBBBBBBBBBBBBBBB            iBBBBBBBQBQBBBBBBBB                       iBBv  iiiiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiiiii  PQP                       rBBBBQBBBQBBBBBBBBBBi           uBBBBBBBBBBQBBBBBBBB                        iBBv  iiiiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiiiii  IBP                                                                                                       iBBs  iiiiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiiiii  kBg                                                                                                           iQBY  iiiiiiiiiiiiiii \n");
	printf("i iiiiiiiiiiii  JBR                                                                                                                QQu  iiiiiiiiiiiii \n");
	printf("i iiiiiiiiii  LBB                                                                                                                    MBu  iiiiiiiiiii \n");
	printf("i iiiiiiii  vBBi                                                                                                                       RBk  iiiiiiiii \n");
	printf("i iiiiii  rBBi                                                        [ 미로 ]                                                           gBU  iiiiiii \n");
	printf("i iiii  iBQr                                                                                                                               gBV   iiii \n");
	printf("i ii  iQBr                                                                                                                                   dBV  iii \n");
	printf("i   iQBv                                                                                                                                       EBq  i \n");
	printf("i igBY                                                                                                                                           PBKi \n");
	printf("iJQr                                                                                                                                               vRv\n");
}

void art_1F()
{
	printf("          B             qB    BB   BBi                                                                            EBr  Bu    Bi            BQ         \n");
	printf("          Q             kB    BQ YViKLSGJSLGS;GJGS;EGJSELGLKFKKFFKGSJGHSFHGJSFGKAHGJSJFGJJGJHJGJFJGJFGAJGSJFJGJHGJJGXu Bv    Bi            BQ         \n");
	printf("          B             qB                                                                                                   Bi            BM         \n");
	printf("          B             PB    EQjUKVVuBgUIXSXSKSXSKSXSKXKVXSXXqSKSKSKVXSKXqSKSqXXVqSXXKSqSqSXSqSqXqSKSXXKSKSKIkbBuVSXksBr    Bi            BQ         \n");
	printf("          B             KB    QB      Bg                                                                       vB      Bk    Bi            BR         \n");
	printf("          B             PB    BQ      Bd                                                                       rB      Bu    Bi            BR         \n");
	printf("          B             qB    BB      BQ                                                                       uB      BJ    Bi            BM         \n");
	printf("          B             PB    BB     PBgPZgggDMDgDgDgDgggDgDgDgggggDgDMDgDMDgDgDgDgDgDgDgggDgggggDMDgggDgDgDgEdZBQr    Bj    Bi            BR         \n");
	printf("          B             KB    BB  iBBj                                                                           iQBv  Bs    Bi            BM         \n");
	printf("          B             qB    BQ dMi                                                                                qMiQu    Bi            BQ         \n");
	printf("          B             qB    gBBQvvJUuIkVkVIVkIIVkVIIIVkIkVkVkVkVIVIIISkVkVkIkVkVkSkVkVkVIVIVkVkVIVIVkSkSIVkVkIukuviKQBr    Bi            BM         \n");
	printf("          B             PB                                                                                                   Bi            BR         \n");
	printf("          Q             KB    KBLUXXkEBUVKXKKqXqXqSKXqXqXqXqXqXqXqKqSKXqXKSqKqXqSKXqXqXqSqXqXqXqXqXqSqXqKqXqSXuQBkIKSSjBr    Bi            BM         \n");
	printf("          B             PB    BB     rB                                                                        qB      BV    Bi            QQ         \n");
	printf("          B             XB    QB     rB                                [ 진열장 ]                               KB      Bj    Bi            BM         \n");
	printf("          B             KQ    BB     sB                                                                        BB      Bu    Bi            BR         \n");
	printf("          Q             VB    BB   iMBrrrvvvvvvvvvrvrvrvvvrvvvvvrvvvvvvvvvrvvvvvvvrvrvrvvvvvvvrvrvvvrvrvvvvvrvriUBd    BJ    Bi            BZ         \n");
	printf("         BBPdEgDgDgDgZZVBQ    BM iQRi                                                                             vQM  Bu    BMKDDgggDgDgEPBBi        \n");
	printf("       qBL              IB    BBQBv                                                                                 uBBBU    Bi              BB       \n");
	printf("      BB                KB    ikVrvuIISISISVXVSVSVXVSISISVXVSVXISVSIXVXVSVSIXVXISIXVSVXVSVXVSVSVSVXVSVSIXVSVXVSVSIIuvrXIi    Bi               PQI     \n");
	printf("    VBu                 PB                                                                                                   Bi                 BB    \n");
	printf("   BB                   dB                                                                                                   Qv                  LBd  \n");
	printf(" vBS                     IUXqPKPqPKPqPKPKqqPqPKPKPKPqqqPKPqPKPKPKPqPKPKPKqqPqPKPqPKqKPXPqPKPKPqqKPKPKPKqKPKPKPKPqPqPqPqqqPqqkdi                    BB \n");
	printf("BB                                                                                                                                                  vB\n");
	printf("i                  iJKVXQQ                                              [ 인형 ]                                                                      \n");
	printf("            iLIVLvZIrii  QS                                                       Skr                                           KUv                   \n");
	printf("        iEQMVviii   DBB  BB                                     DEPZi           gB  qBi                                        Bv LMgi                \n");
	printf("      vBQi               kgEdur     vKqXBQ                    iBv   BB         BB vXiiBP                       ii             BB YsvvBBuqKXjr         \n");
	printf("     gB                      ruERRQQSii  B                   iB  BBv PBEZDZZPPSQ  Xjg  uJqDRPr                BgvXgZi     ivJIBi RIQ      ivkMBD      \n");
	printf("     B             BBB              IDB  Q                rRMI   iii                        iDBY             BB    iBQKZMgPsr                  vBB    \n");
	printf("     BB            kRE                i  BPi             BBi                      KP           BR            B  QBQ                              BB   \n");
	printf("      QBv              PBBBq   iBBi       iBB           BP          ZBQ          iBBi           Bi         dRs                     BBB            Bi  \n");
	printf("       iBBY            bBBXv    QQi         QB         BB           QBB   BBBBQ                 Bi        BE          BBB   iiiYY   i            jB   \n");
	printf("          gBgv                               Qi        QB                  RBs                iBd        Bq           QBB   BBBBJ               EBi   \n");
	printf("            iVgQZIi                         iB          BBi                                 LBBi        iB                    q              idBg     \n");
	printf("                 rSZQZXi                   sBv           rRREYi                      irLXEQgk            B                               ikgBgv       \n");
	printf("                      LMMRbUri          ibBB                 vkdEgZPPdPEbZEgZDdQgDEdKSYvi                qBr                     irsUPDQgKr           \n");
	printf("                          iiuSEDgggZgDgESi                                                                igBZkri        irLKdggDKuvri                \n");
	printf("                                                                                                               irvvsJujvvvii                          \n");
}

void art_B1F()
{
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("              iukuSVSISISISVSVXISVXVSJi                        YbSPPdPbPPPbPbPdPbPdPPPdi                         vdXqPdPdPdPbPbPbPdPbPPqEr            \n");
	printf("             BQr                     vBB                     iQB                      iBB                       BBi                     iBB           \n");
	printf("            BQ                        iQM                    BB                        iBR                     BB                         BB          \n");
	printf("      iIkviBBi                         rBQvSPKi        IBBbVBB                          rBBgBBBi         JQBZVBB                          iBBgBBBv    \n");
	printf("      uQBQBBi                           UBBQBBr        vMXBBB                            KBBDPX          rgKBQBi                           vBBRPqi    \n");
	printf("         uQBVXdgDggRgMgRMRMQRRdZERMRMRDqbBBv            iiPBBgBBBBBBQBQBBBBBBBQBQBBBQBBBQBBBvi            iiIBBMBBBBBBBBBBBBBBBBQBBBBBBBBBBQQBjii     \n");
	printf("     LBQBQBBBBBBBBBBBBBBBQBBBQBBBBBBBBBBBBBQBBBr      BBBBBBBBBBBBBBBBBBBBBBBBBBBQBBBBBBBBBBBBBBr       dQBBBBBBBQBQBQBBBQBBBBBBBQBBBBBBBBBBBQBBBBj   \n");
	printf("     BBBBBBBBBBBEMMQMRMRgRgMgRgRgRggbBQBBBBBBBQB      BBBBBBBBBBMISKXqXqXKSKSXXKSXSXkkBBBBBBBBBBP       BBBBBBBBBBBISKXKXPSKXKSKXqXXSXkkBBBBBBBBBBB   \n");
	printf("     BBBBi   VBK  i i     i i i i    MBv   rBBBR      BBBM    LBu   i i i i i i i i   BB    iBBBI       BBBB    iQb   i i i i i i i i   QBi    BBBQ   \n");
	printf("     BBBv     BMiiiiiiiiiiiiiiiiiiiiiBB     KBBE      BBBi     BSiiiiiiiiiiiiiiiiiiiiiBZ     BBBL       QBBv     BgiiiiiiiiiiiiiiiiiiiiiBB     PBBP   \n");
	printf("     BBBBi   XBb  i iiiiiiiiiii i i  gBk   YBBBM      BBBBkiiiBBU i   iii i i i i i   BBMiiiQBBBq       BBBBqiiiBBd     i i i i i i i   RBBiiigBBBB   \n");
	printf("     BBBBBBBBBBBVqbgggDgDgDgDgDggMZZPBBBQBBBBBBB      BBBBBBQBBBBgRBBBBBBBQBBBBBBBBBBBBBQBBBBBBBP       QBBBBBBBQBBgRBBBBBBBBBBBBBBBBBBBBBBBQBBBQBQ   \n");
	printf("     iBBBBBBBBBBBQBBBQBBBQBBBQBBBBBBBBBBBBBBBBB       igBBBQBRRgBBBBBBBQBBBBBBBBBQBBBBBgggQQBBBJ         EBBBBBMRgBBBQBBBQBQBBBBBBBBBBBBBMMDBQBBBS    \n");
	printf("         LKVvrvE                     DvvvVPr              VDquLXb                     BkYIbQi               vgquLIM                     QVYUbQr       \n");
	printf("         iBBBBBB                     BBBBQBi              rBBBBBY                     BQBBBB                iBBBBBd                     gBBBBBi       \n");
	printf("          iiiii                       iiii                  iiii                        iii                  iiii                         iiii        \n");
	printf("LvvvvvvvvriiiiiivvLvvvLvvvvvvvLvvvLvviiiiiirvvvvvvvvvvvvvvviiiiirvvvvvYvvvvvvvLvvvvvvvriiiiivvvvvvvvvvLvvvvvviiiiirvvvvvLvvvvvvvvvvvvvvvriiiiivvvvLvvv\n");
	printf("LvvvvvvvvriiiiiivvLvvvLvvvvvvvLvvvLvviiiiiirvvvvvvvvvvvvvvviiiiirvvvvvYvvvvvvvLvvvvvvvriiiiivvvvvvvvvvLvvvvvviiiiirvvvvvLvvvvvvvvvvvvvvvriiiiivvvvLvvv\n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                     [ 빨간색 차 ]                                     [ 노란색 차 ]                                    [ 파란색 차 ]                   \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
	printf("                                                                                                                                                      \n");
}

int finditem(int itemcode){
	for(int i = 0; i < 2; i++){
		if(item[i] == itemcode){
			 return i;
		}
	}
	return 3;
}

void printingame(){
	switch(pi){
		case 0: case 1:
			for(int i = 0; i < 40; i++) printf("\n");
			break;
		case 2: //B3F감옥
			art_B3F_1();
			break;
		case 3: //3층 레스토랑
			art_3F();
			break;
		case 4: //감옥 밖
			art_B3F_2();
			break;			
		case 5: //2층 서재
			art_2F();
			break;
		case 6: //지하 2층 미로
			art_B2F();
			break;			
		case 7: //1층 키즈룸
			art_1F();
			break;
		case 8: //지하 1층 주차장
			art_B1F();
			break;			
		case 9: case 10:
			printf("플레이타임 : %s", playtime);
			for(int i = 0; i < 38; i++) printf("\n");
			break;
		default:
			for(int i = 0; i < 40; i++) printf("\n");
	}
	for(int i = 0; i < 150; i++) printf("■");
	printf("\n");
}

void printitem(){
	printf("? : 아이템 조사   ! : 아이템 획득   ~ : 아이템 사용   > : 작은 가방에 아이템 넣기   < : 작은 가방에서 아이템 가져오기   = : 비밀번호 입력하기\n");
	printf("아이템 : ");
	for(int i = 0; i < 2; i++){
		switch(item[i]){
			case 0:
				printf("스테이크 ");
				break;
			case 1: 
				printf("쇠젓가락 ");
				break;
			case 2:
				printf("찬장 열쇠 ");
				break;
			case 3:
				printf("고무장갑 ");
				break;
			default:
				printf("");
		}
	}
	printf("\n");
	for (int i = 0; i < 150; i++) printf("-");
	printf("\n");
}

void printscript(){
	switch(ps){
		case 0:
			printf("지금부터 거대한 건물에서 방탈출 게임이 시작됩니다.\n");
			printf("당신은 지하에서 다른 사람은 지상에서 게임이 진행됩니다.\n");
			printf("무전기 덕에 다른 사람과 소통이 가능하며 마법 가방으로 획득한 아이템을 다른 사람과 공유 가능합니다.\n");
			printf("아이템은 최대 2개까지 획득 가능하며 작은 가방엔 1개만 넣을 수 있습니다. 엔터 키를 누르면 시작합니다.\n");
			break;
		case 1:
			printf("지금부터 거대한 건물에서 방탈출 게임이 시작됩니다.\n");
            printf("당신은 지하에서 다른 사람은 지상에서 게임이 진행됩니다.\n");
            printf("무전기 덕에 다른 사람과 소통이 가능하며 마법 가방으로 획득한 아이템을 다른 사람과 공유 가능합니다.\n");
            printf("아이템은 최대 2개까지 획득 가능하며 작은 가방엔 1개만 넣을 수 있습니다.엔터 키를 누르면 시작합니다.\n");
			break;
		case 2: //지하 3층 감옥 진입
			printf("당신은 지하 3층 감옥에 갇혔습니다.\n");
			printf("지상에 간 사람에게 감옥 문의 암호가 전달되었을테니, 기다려 주세요.\n\n\n");
			break;
		case 20: //문 조사
			printf("잠겨있다.\n\n\n\n");
			break;
		case 3: // 3층 레스토랑 진입
			printf("당신은 문이 잠긴 3층 레스토랑에 있습니다.\n");
			printf("지하 감옥의 암호는 [134]입니다. 채팅으로 상대에게 전달하세요.\n\n\n");
			break;
		case 30: //테이블 조사
			printf("[스테이크]와 [쇠젓가락]이 있다.\n\n\n\n");
			break;
		case 31: //찬장 조사
			printf("잠겨있다.\n\n\n\n");
			break;
		case 32: //문 조사
			printf("잠겨있다.\n");
			printf("(레스토랑 전화번호로 전화하세요.)\n\n\n");
			break;
		case 33: //찬장 열기
			printf("찬장이 열렸다.\n");
			printf("[고무장갑]이 있다.\n\n\n");
			break;
		case 4:
			printf("감옥 문이 열렸다.\n\n\n\n");
			break;
		case 40:
			printf("몬스터가 계단 앞을 지키고 있다.\n");
			printf("이대로는 위층으로 올라갈 수 없다.\n\n\n");
			break;
		case 41:
			printf("무언가를 꽂으면 전기가 통할 것 같다.\n\n\n\n");
			break;
		case 42:
			printf("열쇠 하나가 떨어져 있다. 찬장 열쇠로 보인다.\n\n\n\n");
			break;
		case 43:
			printf("스테이크를 쇠젓가락에 꽂았다.\n");
			printf("파지지직!! 괴물이 다가오더니 감전됐다.\n");
			printf("괴물의 목에 걸린 [목걸이]가 반짝인다.\n\n");
			break;
		case 44: 
			printf("쇠젓가락을 콘센트에 꽂았다.\n");
			printf("잘하면 괴물을 감전시킬 수 있을 것 같다.\n\n\n");
			break;
		case 45:
			printf("고무장갑을 착용했다. 전기에 감전되지 않을 것 같다.\n\n\n\n");
			break;
		case 46:
			printf("목걸이에 전화번호가 적혀있다.\n");
			printf("(복실이 - 01064030572 레스토랑 전화번호로 전화주세요!)\n\n");
			printf("지상의 플레이어가 레스토랑을 탈출하면 엔터키를 눌러 다음 장소로 진입하세요.\n");
			break;
		case 5:
			printf("2층 서재에 진입헀다.\n\n\n\n");
			break;
		case 50:
			printf("각종 책들이 꽂혀 있다.\n");
			printf("(아이 러브 유 왼 유 키스 미) (오늘의 타로) (베스트 오브 클래식)\n");
			printf("(보통 왼손잡이는 고집이 세다) (파트리지오, 그의 여행기) (제 왼발을 고쳐주세요)\n\n");
			break;
		case 51:
			printf("문에 뭔가가 적혀있다.\n");
			printf("(레스토랑에서 키우는 애완 몬스터의 이름을 입력하세요.)\n\n\n");
			break;
		case 52:
			printf("문이 열렸습니다.\n");
			printf("지하의 사람이 문제를 해결한 후 엔터 키를 눌러 다음으로 넘어가세요.\n\n\n");
			break;
		case 6:
			printf("지하 2층에 오니 미로가 있다.\n\n\n\n");
			break;
		case 60:
			printf("'왼쪽 또는 '오른쪽'을 입력해보자. ex)오른쪽 왼쪽 오른쪽 왼쪽 오른쪽 왼쪽\n\n\n\n");
			break;
		case 61:
			printf("미로를 탈출했습니다.\n");
			printf("지상의 사람이 문제를 해결한 후 엔터 키를 눌러 다음으로 넘어가세요.\n\n\n");
			break;
		case 7:
			printf("1층 아이방에 도착했다.\n");
			printf("지하로 통하는 문을 열면 될 것 같다.\n\n\n");
			break;
		case 70:
			printf("인형을 알맞은 순서로 진열해야 할 것 같다.\n\n\n\n");
			break;
		case 71:
			printf("보라, 노랑, 파랑, 파랑, 주황, 초록, 보라, 빨강, 주황, 노랑색의 인형이 있다.\n\n\n\n");
			break;
		case 72:
			printf("문이 열린 것 같다.\n\n\n\n");
			break;
		case 8:
			printf("지하 1층 주차장에 도착했다.\n\n\n\n");
			break;
		case 80:
			printf("빨간색 차 : 62혼194\n\n\n\n");
			break;
		case 81:
			printf("노란색 차 : 68색903\n\n\n\n");
			break;
		case 82:
			printf("파란색 차 : 75합210\n\n\n\n");
			break;
		case 9:
			printf("탈출에 성공했다.\n\n\n\n");
			break;
		case 10:
			printf("탈출에 성공했다.\n\n\n\n");
			break;
		case 1000: //아이템을 잘못 사용했을 때
			printf("아이템을 사용할 수 없습니다.\n\n\n\n");
			break;
		case 1001: //아이템이 꽉 찼을 때
			printf("인벤토리에 빈 공간이 없습니다.\n\n\n\n");
			break;
		case 1002: //작은 가방이 꽉 찼을 때
			printf("작은 가방이 꽉 찼습니다.\n\n\n\n");
			break;
		case 1003: //작은 가방이 비었을 때
			printf("작은 가방이 비었습니다.\n\n\n\n");
			break;
		case 1004: //잠겨 있을 때
			printf("문이 잠겨 있습니다.\n\n\n\n");
			break;
		case 1005: //오브젝트를 조사할 수 없을 
			printf("조사할 수 없는 오브젝트입니다.\n\n\n\n");
			break;
		case 1006: //획득할 아이템이 없을 때
			printf("아이템을 획득할 수 없습니다.\n\n\n\n");
			break;
		case 1007:
			printf("작은 가방에 아이템을 넣을 수 없습니다.\n\n\n\n");
			break;
		case 1008:
			printf("암호가 틀렸습니다.\n\n\n\n");
			break;
		case 1009:
			printf("인벤토리에 해당 아이템이 없습니다.\n\n\n\n");
			break;
		case 1010:
			printf("bag : %c\n\n\n\n", bag[0]);
			break;
		case 1011:
			printf("아이템을 획득했습니다.\n\n\n\n");
			break;
		case 1012:
			printf("암호를 입력할 수 없습니다.\n\n\n\n");
			break;
		default:
			for(int i = 0; i < 5; i++) printf("\n");
	}
	for(int i = 0; i < 150; i++) printf("■");
	printf("\n");
}

int input(char role, int rd, int wr){
	char msg[MAXLINE];
	char msgcom[MAXLINE];
	memset(msgcom, 0, sizeof(msgcom)); //결합시킬 문자열을 초기화
	char buffer[MAXSHM];
	memset(buffer, 0 , sizeof(buffer)); //버퍼를 초기화
	int msgcount = 0;
	printf("입력 : ");
	fgets(msg, MAXLINE, stdin);
	if(msg[0] == '?'){ //오브젝트 조사 조사가 불가능하면 1005
		char order[MAXLINE]; //명령이 뭔지 조사
		for(int i = 0; i < MAXLINE - 1; i++){
			order[i] = msg[i+1];
		}

		if(pi == 2){ //지하 3층 감옥
			if(!strcmp(order, "감옥\n")){
				ps = 20;
			}
			else
			{
				ps = 1005;
			}	
		}
		else if(pi == 3){ // 3층레스토랑
			if(!strcmp(order, "테이블\n")){
				ps = 30;
			}
			else if(!strcmp(order, "찬장\n")){
				ps = 31;
			}
			else if(!strcmp(order, "문\n")){
				ps = 32;
			}
			else {
				ps = 1005;
			}
		}
		else if(pi == 4){ //감옥 밖
			if(!strcmp(order, "계단\n")){
                ps = 40;
            }
            else if(!strcmp(order, "콘센트\n")){
                ps = 41;
            }
            else if(!strcmp(order, "바닥\n")){
                ps = 42;
            }
			else if(!strcmp(order, "목걸이\n")){
				ps = 46;
			}
            else{
                ps = 1005;
            }

		}
		else if(pi == 5){//서재
			if(!strcmp(order, "책장\n")){
				ps = 50;
			}
			else if(!strcmp(order, "문\n")){
				ps = 51;
			}
			else {
				ps = 1005;
			}
		}
		else if(pi == 6){//미로
            if(!strcmp(order, "미로\n")){
                ps = 60;
            }
            else {
                ps = 1005;
            }
        }
		else if(pi == 7){
			if(!strcmp(order, "진열장\n")){
                 ps = 70;
            }
			else if(!strcmp(order, "인형\n")){
                ps = 71;
            }
            else {
                ps = 1005;
            }
		}
		else if(pi == 8){
			if(!strcmp(order, "빨간색 차\n")){
                ps = 80;
            }
            else if(!strcmp(order, "노란색 차\n")){
                ps = 81;
            }
			else if(!strcmp(order, "파란색 차\n")){
                ps = 82;
            }
            else {
                ps = 1005;
            }
		}
		else{
			ps = 1005;
		}

	}
	else if(msg[0] == '!'){ //아이템 획득 인벤토리가 꽉차면 1001 획득할 수 없으면 1006
		char order[MAXLINE]; //명령이 뭔지 조사
        for(int i = 0; i < MAXLINE - 1; i++){
            order[i] = msg[i+1];
        }

		if(pi == 2){ //지하 3층 감옥
            ps = 1006;
		}
		else if(pi == 3){ //3층 레스토랑
			if(itemfull < 2){
	            if(!strcmp(order, "스테이크\n")&&getsteak == 0){
					if(item[0] == 100) //인벤토리 칸이 비었는지 확인
					{
						item[0] = 0;
						itemfull++;
					}
					else if(item[1] == 100)
					{
						item[1] = 0;
						itemfull++;
					}
					ps = 1011;
					getsteak = 1;
	            }
				else if(!strcmp(order, "쇠젓가락\n")&&getchopstick == 0){
					if(item[0] == 100)
                    {
                        item[0] = 1;
                        itemfull++;
                    }
                    else if(item[1] == 100)
                    {
                        item[1] = 1;
                        itemfull++;
                    }
					ps = 1011;
					getchopstick = 1;
				}
				else if((!strcmp(order, "고무장갑\n")&&cupboard == 1) && getglove == 0){
					if(item[0] == 100)
                    {
                        item[0] = 3;
                        itemfull++;
                    }
                    else if(item[1] == 100)
                    {
                        item[1] = 3;
                        itemfull++;
                    }
					ps = 1011;
					getglove = 1;
				}
				else
				{
					ps = 1006;
				}
			}
			else
			{
				ps = 1001;
			}
		}
		else if(pi == 4){
			if(itemfull < 2){
				if(!strcmp(order, "찬장 열쇠\n")&&getcupboard == 0){
                    if(item[0] == 100) //인벤토리 칸이 비었는지 확인
                    {
                        item[0] = 2;
                        itemfull++;
                    }
                    else if(item[1] == 100)
                    {
                        item[1] = 2;
                        itemfull++;
                    }
					ps = 1011;
					getcupboard = 1;
                }
				else{
					ps = 1006;
				}

			}
		}
		else{
			ps = 1006;
		}
	
	}
	else if(msg[0] == '~'){ //아이템 사용. 사용 불가능할 경우 1000
		char order[MAXLINE]; //명령이 뭔지 조사
        for(int i = 0; i < MAXLINE - 1; i++){
            order[i] = msg[i+1];
        }
		if(pi == 2){ //지하 3층 감옥
			ps = 1000;
		}
		else if(pi == 3){ // 3층 레스토랑
			if(!strcmp(order, "찬장 열쇠\n")){
				int i = finditem(2);
				if(i == 3) { //인벤토리에 아이템이 없다면
					ps = 1000;					
				}
				else {
					cupboard = 1;
					item[i] = 100;
					itemfull--;
					ps = 33;
				}
				
			}
			else {
				ps = 1000;
			}
		}
		else if(pi == 4){
			if(!strcmp(order, "스테이크\n")&& chopstick == 1){
                 int i = finditem(0);
                 if(i == 3) { //인벤토리에 아이템이 없다면
                     ps = 1000;
                 }
                 else {
                    item[i] = 100;
                    itemfull--;
					steak = 1;
                    ps = 43;
                 }
 
            }
			else if(!strcmp(order, "쇠젓가락\n")&& glove == 1){
                int i = finditem(1);
                if(i == 3) { //인벤토리에 아이템이 없다면
                    ps = 1000;
                }
                else {
                    item[i] = 100;
                    itemfull--;
					chopstick = 1;
                    ps = 44;
                }
 
            }
			else if(!strcmp(order, "고무장갑\n")){
                 int i = finditem(3);
                 if(i == 3) { //인벤토리에 아이템이 없다면
                     ps = 1000;
                 }
                 else {
                     item[i] = 100;
                     itemfull--;
                     glove = 1;
                     ps = 45;
                 }
 
             }

            else {
                ps = 1000;
            }

		}
		else{
			ps = 1000;
		}

    }
	else if(msg[0] == '>'){ //획득한 아이템을 공유 인벤토리로 이동. 꽉 찬 경우 1002 .넣을 수 없을 경우 1007. 개인 인벤토리가 빈 경우1009
		char order[MAXLINE]; //명령이 뭔지 조사
        for(int i = 0; i < MAXLINE - 1; i++){
            order[i] = msg[i+1];
        }

		char bagitem[1];
		memset(bagitem, 0, sizeof(bagitem));
		if(bag[0] == '\0'){ //가방이 비었다면
			if(pi == 2){ //지하감옥
				ps = 1007;
			}
			else if(pi == 3){ //3층 레스토랑
				if(!strcmp(order, "스테이크\n")){
					int i = finditem(0);
					if(i == 3){ //인벤토리에 아이템이 없다면
						ps = 1009;
					}
					else{
						sprintf(bagitem, "%d", item[i]);
						memset(bag, 0, sizeof(bag));
						bag[0] = bagitem[0];
						item[i] = 100;
						itemfull--;
					}
				}
				else if(!strcmp(order, "쇠젓가락\n")){
					int i = finditem(1);
					if(i == 3) {
						ps = 1009;
					}
					else{
						sprintf(bagitem, "%d", item[i]);
						memset(bag, 0, sizeof(bag));
						bag[0] = bagitem[0];
						itemfull--;
						item[i] = 100;
					}
				}
				else if(!strcmp(order, "고무장갑\n")){
					int i = finditem(3);
					if(i == 3){
						ps = 1009;						
					}
					else{
						sprintf(bagitem, "%d", item[i]);
						memset(bag, 0, sizeof(bag));
						bag[0] = bagitem[0];
						itemfull--;
						item[i] = 100;
					}
				}
				else {
					ps = 1007;
				}
			}
			else if(pi == 4) {
				if(!strcmp(order, "찬장 열쇠\n")){
                    int i = finditem(2);
                    if(i == 3){ //인벤토리에 아이템이 없다면
                        ps = 1009;
                    }
                    else{
                        sprintf(bagitem, "%d", item[i]);
						memset(bag, 0, sizeof(bag));
                        strcpy(bag, bagitem);
                        item[i] = 100;
                        itemfull--;
                    }
                }
				else {
					ps = 1007;
				}
			}
			else{
				ps = 1007;
			}
		}
		else{
			ps = 1002;
		}
    }
	else if(msg[0] == '<'){ //공유 인벤토리의 아이템을 가져옴. 인벤토리 꽉 차면 1001 공유 인벤 비었으면 1003
		char order[MAXLINE]; //명령이 뭔지 조사
        for(int i = 0; i < MAXLINE - 1; i++){
			
            order[i] = msg[i+1];
        }
		int i = bag[0] - '0';
		if(itemfull < 2){
			if(bag[0]<58 && bag[0]>47){ //가방에 아이템이 있다면
				if(item[0] == 100)
				{
                    item[0] = i;
                    itemfull++;
                }
                else if(item[1] == 100)
                {
                    item[1] = i;
                    itemfull++;
                }
				memset(bag, 0, sizeof(bag));
			}
			else{
				ps = 1003;
			}
		}
		else{
			ps = 1001;
		}
    }
	else if(msg[0] == '='){ //암호 입력. 틀리면 1008
		char order[MAXLINE]; //명령이 뭔지 조사
        for(int i = 0; i < MAXLINE - 1; i++){
            order[i] = msg[i+1];
        }
		if(pi == 2){
			if(!strcmp(psw[pi], order)){ //플레이어가 정답을 맞췄다면
				pi += 2;
				ps = pi;
			}
			else{
				ps = 1008;
			}
		}
		else if(pi == 3){
			if(!strcmp(psw[pi], order)){ //플레이어가 정답을 맞췄다면
                pi += 2;
                ps = pi;
				progress[0] = '1';
            }
            else{
                ps = 1008;
            }
		}
		else if(pi == 5){
			if(!strcmp(psw[pi], order)){ //플레이어가 정답을 맞췄다면
				if(progress[0] == '\0'){
					progress[0] = '1';
					ps = 52;
				}
				else if(progress[0] == '1'){
					progress[0] = '2';
					pi += 2;
					ps = pi;
				}
            }
            else{
                ps = 1008;
            }
		}
		else if(pi == 6){
            if(!strcmp(psw[pi], order)){ //플레이어가 정답을 맞췄다면
                if(progress[0] == '\0'){
                    progress[0] = '1';
					ps = 61;
                }
                else if(progress[0] == '1'){
                    progress[0] = '2';
                    pi += 2;
                    ps = pi;
                }
            }    
            else{
                ps = 1008;
            }   
        }
		else if(pi == 7){
			if(!strcmp(psw[pi], order)){ //플레이어가 정답을 맞췄다면
		        progress[0] = '1';
				pi += 2;
                ps = pi;
				read(rd, playtime, sizeof(playtime));
            }
            else{
                ps = 1008;
            }
		}
		else {
			ps = 1012;
		}
    }
	else if(msg[0] == '\n') {
		if(pi == 1 || pi == 0){ //플레이어의 진행도가 처음이라면.
			starttime = clock(); //게임 시작 시간 재기.
			pi += 2;
			ps += 2;
		}
		return 0;	  
    }
	else {
		for(int i = 0; i < MAXSHM; i++) //메세지 로그가 다 찼는지 검사
		{
			if(chatlog[i] == '\n') {
				msgcount++;		
			}
		}
		if(msgcount >= 5){ //로그가 다섯 줄이라면.
			int line = 0;
			for(int i = 0; i < MAXSHM; i++) {
				if(line > 0) {
					strncat(buffer, chatlog + i, strlen(chatlog + i)); //가장 오래된 줄을 제외하고 버퍼에 복사
					break;
				}
			
				if(chatlog[i] == '\n')
				{
					line++;
				}
			}
			memset(chatlog, 0, 1024); //공유메모리를 초기화하고
			strcpy(chatlog, buffer); //버퍼를 공유메모리에 복사
		}
		if(role == '1') { //지상인지 지하인지 사용자를 표시하기.
			strcpy(msgcom, "지상 : ");
		}
		else{
			strcpy(msgcom, "지하 : ");
		}
		strcat(msgcom, msg);	
		strcat(chatlog, msgcom);
		return 1;
	}
}

void up(char role, int rd, int wr){
	printf("지상\n");;
	pi = 1; // 진행이 이루어질 때마다 2씩 증가;
	ps = 1;
	while(1){
		if((progress[0] =='2')&&pi == 5 ){ //반대편 상황이 종료될 때 까지 대기.
            if(progress[0] != '\0'){ 
                pi += 2;
                ps = pi; 
                memset(progress, 0, sizeof(progress));
            }
        }
		printf("\033[2J\033[H"); //창 지우기
		printingame();
		printitem();
		printscript();
		if(pi == 9){
			break;
		}			
		input(role, rd, wr); //입력받기		
	}

}

void down(char role, int rd, int wr){
	printf("지하\n");
	pi = 0; //진행이 이루어질 때마다 2씩 증가
	ps = 0;
	while(1) {
		if(steak == 1 || ((progress[0] == '2')&&pi == 6) || (pi == 8&& progress[0] == '1')){ //반대편 상황이 종료될 때 까지 대기.
			if(progress[0] != '\0'){
				steak = 0;	
				pi += 2;
				ps = pi;
				memset(progress, 0, sizeof(progress));
			}
			if(pi == 10){
				finishtime = clock();
				sprintf(playtime, "%d",  finishtime - starttime);
				write(wr, playtime, sizeof(playtime));
				break;
			}
		}
		printf("\033[2J\033[H");
		printingame(); //아마 구분 선 까지 포함해서 41줄
		printitem(); //구분 선까지 포함해서 2줄
		printscript();	//아마 구분 선까지 포함해서 6줄
		input(role, rd, wr); // 1줄		
	}

}

int main() {
	int rd, wr, n, crd, cwr, chatwrite;
	char hg_ans[20];
	char roomname[20];
	char code[20];
	int hostguest = 0; //호스트 게스트를 입력했는지 확인
	
	item[0] = 100;
	item[1] = 100;


	rd = open("./fromserver", O_RDONLY);
	wr = open("./fromclient", O_WRONLY);
	if(rd == -1 || wr == -1) {
		perror("open");
		exit(1);
	}

	while(!hostguest){
		printf("\033[2J\033[H");
		intro();
		printf("호스트(1) 게스트(0) :");
	    fgets(hg_ans, 20, stdin);

		if(!strcmp(hg_ans, "1\n\0")) //호스트일 경우
		{
			signal(SIGINT, intHandlerh);
			printf("\033[2J\033[H");
			intro();
			printf("팀 이름을 입력하세요 : ");			
			fgets(roomname, 20, stdin); 
			srand(time(NULL));
			int c = rand()%10;
			printf("\033[2J\033[H");
			intro();
			printf("방 코드 : %d\n", c);
			sprintf(code, "%d", c); //방코드 뽑기

			write(wr, hg_ans, 1); //호스트인지 게스트인지 보내고
			write(wr, code, 1); //방코드와 방이름 보내기
			write(wr, roomname, MAXLINE);


			//방이름으로 클라이언트끼리 잇는 파이프 생성
			strncat(hpipe, roomname, sizeof(roomname) - 2);
			strncat(hpipe, code, 1); //
			strncat(gpipe, roomname, sizeof(roomname) - 2);
			strncat(gpipe, code, 1); //
			strncat(cpipe, hg_ans, sizeof(hg_ans));
			strncat(cpipe, roomname, sizeof(roomname) - 2);
			strncat(cpipe, code, 1); //
			if(mkfifo(hpipe, 0666) == -1) {
				perror("hostpipe");
				exit(1);
			}
			if(mkfifo(gpipe, 0666) == -1) {
				perror("guestpipe");
				exit(1);
			}
			if(mkfifo(cpipe, 0666) == -1) {
                 perror("chatpipe");
                 exit(1);
            }


//			printf("hpipe : %s gpipe : %s cpipe : %s\n", hpipe, gpipe, cpipe);

			crd = open(gpipe, O_RDONLY); //파이프 열기
			cwr = open(hpipe, O_WRONLY);
			chatwrite = open(cpipe, O_WRONLY);			

			if(crd == -1 || cwr == -1 || chatwrite == -1) {
				perror("클아리언트 파이프 오류");
			}

		    key_t chat_key;
			key_t bag_key;
			key_t progress_key;
 
		    chat_key = ftok(hpipe, roomname[0]); //채팅파이프 공유메모리 키 생성 나중에 해싱?으로 방이름으로 만드는 거 생각해보자.
			bag_key = ftok(gpipe, roomname[0]);			
			progress_key = ftok(cpipe, roomname[0]);			

			if(chat_key == -1) {
				perror("chatftok");
				return 1;
			}
			if(bag_key == -1){
				perror("bagftok");
				return 1;
			}
			if(progress_key == -1){
				perror("progressftok");
				return 0;
			}

		    chat_shmid = shmget(chat_key, 1024, IPC_CREAT|0666); //공유 메모리에 접근
			bag_shmid = shmget(bag_key, 300, IPC_CREAT|0666);
			progress_shmid = shmget(progress_key, 300, IPC_CREAT|0666);	 

		    chatlog = (char *)shmat(chat_shmid, NULL, 0);
			bag = (char *)shmat(bag_shmid, NULL, 0);
			progress = (char *)shmat(progress_shmid, NULL, 0);

		    memset(chatlog, 0, 1024);
			memset(bag, 0, 300);
			memset(progress, 0, 300);
			
//			printf("chat shmid : %d\n", chat_shmid);
//			printf("bag shmid : %d\n", bag_shmid);			

			write(cwr, &chat_shmid, sizeof(chat_shmid));
			write(chatwrite, &chat_shmid, sizeof(chat_shmid));
			write(cwr, &bag_shmid, sizeof(bag_shmid));
			write(chatwrite, &bag_shmid, sizeof(bag_shmid));
			write(cwr, &progress_shmid, sizeof(progress_shmid));			
			
			srand(time(NULL));
			int role = rand()%2; //호스트가 지상인지 지하인지 뽑기
			
			if(role == 1) { //지상
				char r[1] = "0";
				write(cwr, r, 1);
				up('1', crd, cwr);
			}
			else{
				char r[1] = "1";
				write(cwr, r, 1);
				down('0', crd, cwr);
			}
			

		}
		else if(!strcmp(hg_ans, "0\n\0")) //게스트일 경우
		{	
			signal(SIGINT, intHandlerg);
			char code[20];
			while(1) {
				printf("\033[2J\033[H");
				intro();
				printf("코드를 입력하세요. : ");
				memset(code, 0, sizeof(code));
				fgets(code, 20, stdin); //방코드 입력
				if(code[1] != '\n'){
					printf("\033[2J\033[H");
					continue;
				}
				write(wr, hg_ans, 1); //서버에 게스트라고 보낸 후
				write(wr, code, MAXLINE); //게스트의 방코드를 보내기
				memset(roomname, 0, sizeof(roomname));
				read(rd, roomname, MAXLINE);
				printf("\033[2J\033[H");
				intro();
				printf("방 이름 : %s\n", roomname);
				if(roomname[0] != '\0') //입력한 코드가 맞아 방이름을 받았다면
				{
//					printf("방 코드 입력 성공\n");
		            strncat(hpipe, roomname, sizeof(roomname) - 2);
					strncat(hpipe, code, 1);//
		            strncat(gpipe, roomname, sizeof(roomname) - 2);
					strncat(gpipe, code, 1);//
		            strncat(cpipe0, roomname, sizeof(roomname) - 2);
					strncat(cpipe0, code, 1);//
//					printf("hpipe : %s gpipe : %s cpipe : %s\n", hpipe, gpipe, cpipe0);

					if(mkfifo(cpipe0, 0666) == -1) {
                         perror("chatpipe");
                         exit(1);
                    }

					cwr = open(gpipe, O_WRONLY); //파이프 열기
					crd = open(hpipe, O_RDONLY);
					chatwrite = open(cpipe0, O_WRONLY);

					if(cwr == -1 || crd == -1 || chatwrite == -1) {
						perror("클라이언트 파이프 열기 오류");
					}					
					

					read(crd, &chat_shmid, sizeof(chat_shmid));
					read(crd, &bag_shmid, sizeof(bag_shmid));
					read(crd, &progress_shmid, sizeof(progress_shmid));

					printf("chat shmid : %d\n", chat_shmid);
					printf("bag shmid : %d\n", bag_shmid);

					chatlog = (char *)shmat(chat_shmid, NULL, 0);
					bag = (char *)shmat(bag_shmid, NULL, 0);
					progress = (char *)shmat(progress_shmid, NULL, 0);

					write(chatwrite, &chat_shmid, sizeof(chat_shmid));
					write(chatwrite, &bag_shmid, sizeof(bag_shmid));

					char r[1];
					read(crd, r, 1);
					if(r[0] == '1') {
						up(r[0], crd, cwr);
					}
					else {
						down(r[0], crd, cwr);
					}

					break;
				}
				printf("\033[2J\033[H");
			}
		}
		else //다른 입력일 경우
		{
			printf("\033[2J\033[H");
		}
	}

}

