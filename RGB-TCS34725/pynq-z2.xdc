set_property -dict { PACKAGE_PIN R16   IOSTANDARD LVCMOS33 } [get_ports { IIC_0_scl_io }]; #IO_L19P_T3_34 Sch=ar[6]
set_property -dict { PACKAGE_PIN U17   IOSTANDARD LVCMOS33 } [get_ports { IIC_0_sda_io }]; #IO_L9N_T1_DQS_34 Sch=ar[7]


set_property PULLUP true [get_ports IIC_0_scl_io]
set_property PULLUP true [get_ports IIC_0_sda_io]