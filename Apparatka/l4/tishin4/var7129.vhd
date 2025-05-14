--
--  File: C:\XILINX\ACTIVE\PROJECTS\TISHIN4\var7129.vhd
-- created: 03/22/25 08:34:09
--  from: 'C:\XILINX\ACTIVE\PROJECTS\TISHIN4\var7129.asf'
--  by fsm2hdl - version: 2.0.1.60
--
library IEEE;
use IEEE.std_logic_1164.all;

use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

library SYNOPSYS;
use SYNOPSYS.attributes.all;

entity var7129 is 
  port (C: in STD_LOGIC;
        d: in STD_LOGIC;
        Reset: in STD_LOGIC;
        RL: in STD_LOGIC;
        start: in STD_LOGIC;
        stop: in STD_LOGIC;
        PL: out STD_LOGIC;
        PR: out STD_LOGIC;
        V: out STD_LOGIC);
end;

architecture var7129_arch of var7129 is


-- SYMBOLIC ENCODED state machine: Sreg0
type Sreg0_type is (S0, S1, S2, S3);
signal Sreg0: Sreg0_type;

begin
-- concurrent signals assignments


Sreg0_machine: process (C, reset)

begin

if Reset='1' then
	Sreg0 <= S0;
elsif C'event and C = '1' then
	case Sreg0 is
		when S0 =>
			if (d and RL and start)='1' then
				Sreg0 <= S2;
			elsif (d and (not RL) and start)='1' then
				Sreg0 <= S3;
			elsif stop='1' then
				Sreg0 <= S0;
			elsif (not d and start)='1' then
				Sreg0 <= S1;
			end if;
		when S1 =>
			if stop='1' then
				Sreg0 <= S0;
			end if;
		when S2 =>
			if (d and (not RL))='1' then
				Sreg0 <= S3;
			elsif ((not d) and (not stop))='1' then
				Sreg0 <= S1;
			elsif stop='1' then
				Sreg0 <= S0;
			end if;
		when S3 =>
			if (d and RL)='1' then
				Sreg0 <= S2;
			elsif ((not d) and (not stop))='1' then
				Sreg0 <= S1;
			elsif stop='1' then
				Sreg0 <= S0;
			end if;
		when others =>
			null;
	end case;
end if;
end process;

-- signal assignment statements for combinatorial outputs
V_assignment:
V <= '1' when (Sreg0 = S1) else
     '0' when (Sreg0 = S2) else
     '0' when (Sreg0 = S3) else
     '0';

PL_assignment:
PL <= '0' when (Sreg0 = S1) else
      '1' when (Sreg0 = S2) else
      '0' when (Sreg0 = S3) else
      '0';

PR_assignment:
PR <= '0' when (Sreg0 = S1) else
      '0' when (Sreg0 = S2) else
      '1' when (Sreg0 = S3) else
      '0';

end var7129_arch;
