library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity display is
    Port ( reset  : in std_logic;
           clk    : in std_logic;
           digit0 : in std_logic_vector(3 downto 0);
           digit1 : in std_logic_vector(3 downto 0);
           digit2 : in std_logic_vector(3 downto 0);
           digit3 : in std_logic_vector(3 downto 0);
           write  : out std_logic;
           addr   : out std_logic_vector(1 downto 0);
           data   : out std_logic_vector(6 downto 0));
end display;

architecture Behavioral of display is
	signal selDigit: std_logic_vector(3 downto 0);
	signal address : std_logic_vector(1 downto 0);
begin
	
control:	
	process (reset,clk)
		variable cnt : std_logic_vector(1 downto 0);
		begin
			if (reset ='0') then
				address <= "11";
				write <= '1';
				cnt := "10";
			elsif falling_edge(clk) then
				if (cnt = "10") then	
					cnt := "00";
				else    
					cnt := cnt + "01";
				end if;
				case cnt is
					when "00" =>   address <= address + "01";
					when "01" =>   write <= '0';
					when others => write <= '1';
				end case;
			end if;
		end process;

addr <= address;

mux:   
	with address select
		selDigit <= Digit0 when "00",
					   Digit1 when "01",
						Digit2 when "10",
						Digit3 when others; 


	
encoder:	
	with selDigit select
		data  <= "0110000" when "0000", -- 0
					"0110001" when "0001", -- 1
					"0110010" when "0010", -- 2
					"0110011" when "0011", -- 3
					"0110100" when "0100", -- 4
					"0110101" when "0101", -- 5
					"0110110" when "0110", -- 6
					"0110111" when "0111", -- 7
					"0111000" when "1000", -- 8
					"0111001" when "1001", -- 9
					"1000001" when "1010", -- A
					"1000010" when "1011", -- B
					"1000011" when "1100", -- C
					"1000100" when "1101", -- D
					"1000101" when "1110", -- E
					"1000110" when others; -- F




end Behavioral;
