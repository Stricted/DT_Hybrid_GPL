[settings]
functionname = hi_kernel_dp_qos_cnt_get_cmd;
attributenum = 18;
attributeflag= 1;

[parameter]
attribute=alias<scar_all>                  type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<scar_cpu_drop>             type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<scar_local_drop>           type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<scar_bc_drop>              type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<scar_unknown_uc_drop>      type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<fcar_all>                  type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<gcar_all>                  type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<fcar_yellow>               type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<fcar_red>                  type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<gcar_yellow>               type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<gcar_red>                  type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<total_share_used_0>        type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<total_share_used_1>        type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<total_share_used_2>        type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<total_share_used_3>        type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<find_qid_fail>             type<uint>     range<0x0,0xFFFFFFFF>   default<0>;      
attribute=alias<cpu_queue_car_drop>        type<binary>   range<0,32>             default<"0x00">; 
attribute=alias<eqg_ss_cnt>                type<binary>   range<0,32>             default<"0x00">; 


[help]
typedef struct
{
 #ifdef CONFIG_HISI_SD5115_MP_PILOT
    uint ui_scar_all;                      /* 参加简单流量抑制的报文个数   */
    uint ui_scar_cpu_drop;                 /* SCAR TO_CPU 丢包             */
    uint ui_scar_local_drop;               /* SCAR本地转发丢包             */
    uint ui_scar_bc_drop;                  /* SCAR广播丢包                 */
    uint ui_scar_unknown_uc_drop;          /* SCAR未知单播丢包             */
    uint ui_fcar_all;                      /* 参与第一级CAR的报文个数      */
    uint ui_gcar_all;                      /* 参与第二级CAR的报文个数      */
    uint ui_fcar_yellow;                   /* 第一级CAR的黄色报文个数      */
    uint ui_fcar_red;                      /* 第一级CAR的红色报文个数      */
    uint ui_gcar_yellow;                   /* 第二级CAR的黄色报文个数      */
    uint ui_gcar_red;                      /* 第二级CAR的红色报文个数      */
    uint ui_total_share_used[4];           /* 队列共享空间已使用统计       */
    uint ui_find_qid_fail;                 /* 报文获取QID失败统计          */

    uint ui_cpu_queue_car_drop[8];         /* CPU队列CAR丢去统计           */
 #else
    uint ui_scar_all;               /* 参加简单流量抑制的报文个数   */
    uint ui_tocpu_rz_car;           /* 参加tocpu_rz_car抑制的报文个数   */
    uint ui_fcar_igr;               /* 参与第一级入口流CAR的报文个数      */
    uint ui_fcar_egr;               /* 参与第一级出口流CAR的报文个数      */
    uint ui_fcar_igr_yellow;        /* 参与第一级入口流CAR黄色报文个数统计寄存器。*/
    uint ui_fcar_egr_yellow;        /* 参与第一级出口流CAR黄色报文个数统计寄存器。*/
    uint ui_fcar_igr_green;         /* 参与第一级入口流CAR绿色报文个数统计寄存器。*/
    uint ui_fcar_egr_green;         /* 参与第一级出口流CAR绿色报文个数统计寄存器。*/
    uint ui_gcar_all;               /* 参与第二级CAR的报文个数      */
    uint ui_gcar_yellow;            /* 第二级CAR的黄色报文个数      */
    uint ui_gcar_green;             /* 第二级CAR的绿色报文个数      */
    uint ui_total_share_used;       /* 队列共享空间已使用统计       */
    uint ui_find_qid_fail;          /* 报文获取QID失败统计          */
 #endif
 
#ifdef CONFIG_HISI_SD5115_MP_PILOT
    #ifdef CONFIG_HISI_SD5115_TYPE_T
    uint    ui_eqg_ss_cnt[2][4];           /* 队列共享空间已使用统计       */
    #else
    uint    ui_eqg_ss_cnt[4];           /* 队列共享空间已使用统计       */
	#endif    
#endif
} hi_dp_qos_cnt_s;
