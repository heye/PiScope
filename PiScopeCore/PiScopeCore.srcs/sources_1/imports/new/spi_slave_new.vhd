----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 01/16/2013 10:11:44 AM
-- Design Name: 
-- Module Name: spi_slave_new - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL; 
use ieee.std_logic_unsigned.all; 

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;


--default is CPHA=1
--           CPOL=0
entity spi_slave_new is
    Generic (   
        N : positive := 8
        --Rx_N : positive := 8;
        --Tx_N : positive := 8;
    );                                          -- 32bit serial word length is default
        --CPOL : std_logic := '0';                                        -- SPI mode selection (mode 0 default)
        --CPHA : std_logic := '0';                                        -- CPOL = clock polarity, CPHA = clock phase.
        --PREFETCH : positive := 3); 
    Port ( 
           reset : in STD_LOGIC;
           spi_miso : out STD_LOGIC;
           spi_mosi : in STD_LOGIC;
           spi_sck : in STD_LOGIC;
           spi_ss : in STD_LOGIC;
           data_tx : in STD_LOGIC_VECTOR(N-1 downto 0);
           data_rx : out STD_LOGIC_VECTOR(N-1 downto 0);
           data_request : out STD_LOGIC;
           data_ready : out STD_LOGIC
    );
end spi_slave_new;

architecture Behavioral of spi_slave_new is
    signal clk : STD_LOGIC;
    signal current_bit_mosi : natural range 0 to 31;
    signal current_bit_miso : natural range 0 to 31;
    
    signal data_in_buffer : STD_LOGIC_VECTOR(N-1 downto 0);
    signal data_out_buffer : STD_LOGIC_VECTOR(N-1 downto 0);
    signal bitM : STD_LOGIC;
    signal data_request_internal : STD_LOGIC;
    
    type transmit_state is (b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23);
    signal current_state : transmit_state;
    signal next_state : transmit_state;
begin
    clk <= spi_sck;
    --data_request <= data_request_internal;
    
    
    --*************************************** empfangsprocess funktioniert auch bei 32MHz noch gut *******************************
    data_capture_mosi : process(clk, current_bit_mosi, data_in_buffer, spi_ss, spi_mosi)
        begin
            if falling_edge(clk) then
                if (current_bit_mosi > 0) and spi_ss = '0' then                
                    --bit N-1 wurde schon gelesen, jetzt N-2, usw....
                    data_in_buffer(current_bit_mosi-1) <= spi_mosi;  --test
                    data_in_buffer(N-1) <= bitM;
                    --rückwärtscounter
                    current_bit_mosi <= current_bit_mosi - 1;
                    
                    --data ready nur einen clock zyklus high lassen
                    data_ready <= '0';
                else
                    --data is ready, write out only if ss 0 
                    if current_bit_mosi = 0 or spi_ss = '0' then
                        --buffer in ausgang schreiben ACHTUNG buffer wird weiter unten direkt wieder beschrieben - bisher aber keine fehler
                        data_rx(N-1 downto 0) <= data_in_buffer(N-1 downto 0);
                        
                        --neue daten am ausgang
                        data_ready <= '1';
                    end if;
                    
                    --rückwärtscounter neu starten
                    current_bit_mosi <= N-1; 
                    
                    --bit der nächten übertragung direkt einlesen ACHTUNG buffer wird hier gleichzeitig geschreiben und oben gelesen - bisher aber keine fehler
                    bitM <= spi_mosi;--data_in_buffer(N-1) <= spi_mosi;
                end if;   
            end if;
        end process;
    
    
--    data_capture_mosi : process(clk, current_bit_mosi, data_in_buffer, spi_ss, spi_mosi)
--    begin
--        if falling_edge(clk) then
--                    
--            
--                    
--            if (current_bit_mosi > 0) and spi_ss = '0' then                
--                --bit N-1 wurde schon gelesen, jetzt N-2, usw....
--                data_in_buffer(current_bit_mosi-1) <= spi_mosi;  --test
--            
--                --rückwärtscounter
--                current_bit_mosi <= current_bit_mosi - 1;
--                
--                --data ready nur einen clock zyklus high lassen
--                data_ready <= '0';
--            else
--                --data is ready, write out only if ss 0 
--                if current_bit_mosi = 0 or spi_ss = '0' then
--                    --buffer in ausgang schreiben ACHTUNG buffer wird weiter unten direkt wieder beschrieben - bisher aber keine fehler
--                    data_rx(N-1 downto 0) <= data_in_buffer(N-1 downto 0);
--                    
--                    --neue daten am ausgang
--                    data_ready <= '1';
--                end if;
--                
--                --rückwärtscounter neu starten
--                current_bit_mosi <= N-1; 
--                
--                --bit der nächten übertragung direkt einlesen ACHTUNG buffer wird hier gleichzeitig geschreiben und oben gelesen - bisher aber keine fehler
--                data_in_buffer(N-1) <= spi_mosi;
--            end if;   
--        end if;
--    end process;
    

    output_states : process(current_state)
    begin
        case current_state is
            when b23 =>
                spi_miso <= data_out_buffer(23);
                data_request <= '0';
            when b22 =>
                spi_miso <= data_out_buffer(22);
                data_request <= '0';
            when b21 =>
                spi_miso <= data_out_buffer(21);
                data_request <= '0';
            when b20 =>
                spi_miso <= data_out_buffer(20);
                data_request <= '0';
            when b19 =>
                spi_miso <= data_out_buffer(19);
                data_request <= '0';
            when b18 =>
                spi_miso <= data_out_buffer(18);
                data_request <= '0';
            when b17 =>
                spi_miso <= data_out_buffer(17);
                data_request <= '0';
            when b16 =>
                spi_miso <= data_out_buffer(16);
                data_request <= '0';
            when b15 =>
                spi_miso <= data_out_buffer(15);
                data_request <= '0';
            when b14 =>
                spi_miso <= data_out_buffer(14);
                data_request <= '0';
            when b13 =>
                spi_miso <= data_out_buffer(13);
                data_request <= '0';
            when b12 =>
                spi_miso <= data_out_buffer(12);
                data_request <= '0';
            when b11 =>
                spi_miso <= data_out_buffer(11);
                data_request <= '0';
            when b10 =>
                spi_miso <= data_out_buffer(10);
                data_request <= '0';
            when b9 =>
                spi_miso <= data_out_buffer(9);
                data_request <= '0';
            when b8 =>
                spi_miso <= data_out_buffer(8);
                data_request <= '0';
            when b7 =>
                spi_miso <= data_out_buffer(7);
                data_request <= '0';
            when b6 =>
                spi_miso <= data_out_buffer(6);
                data_request <= '0';
            when b5 =>
                spi_miso <= data_out_buffer(5);
                data_request <= '0';
            when b4 =>
                spi_miso <= data_out_buffer(4);
                data_request <= '0';
            when b3 =>
                spi_miso <= data_out_buffer(3);
                data_request <= '0';
            when b2 =>
                spi_miso <= data_out_buffer(2);
                data_request <= '0';
            when b1 =>
                spi_miso <= data_out_buffer(1);
                data_request <= '1';
            when b0 =>
                spi_miso <= data_out_buffer(0);
                data_request <= '0';
            when others =>
                spi_miso <= data_out_buffer(0);
                data_request <= '0';
        end case;
    end process; 
    
    
    
    
    
    output_counter : process(spi_sck, current_state, spi_ss)
    begin
        if rising_edge(spi_sck) then
            --current_state <= next_state;
            
            if spi_ss = '1' or reset = '1' then
                current_state <= b23;
            else
            case current_state is
                when b23 =>
                    current_state <= b22;
                when b22 =>
                    current_state <= b21;
                when b21 =>
                    current_state <= b20;
                when b20 =>
                    current_state <= b19;
                when b19 =>
                    current_state <= b18;
                when b18 =>
                    current_state <= b17;
                when b17 =>
                    current_state <= b16;
                when b16 =>
                    current_state <= b15;
                when b15 =>
                    current_state <= b14;
                when b14 =>
                    current_state <= b13;
                when b13 =>
                    current_state <= b12;
                when b12 =>
                    current_state <= b11;
                when b11 =>
                    current_state <= b10;
                when b10 =>
                    current_state <= b9;
                when b9 =>
                    current_state <= b8;
                when b8 =>
                    current_state <= b7;
                when b7 =>
                    current_state <= b6;
                when b6 =>
                    current_state <= b5;
                when b5 =>
                    current_state <= b4;
                when b4 =>
                    current_state <= b3;
                when b3 =>
                    current_state <= b2;
                when b2 =>
                    current_state <= b1;
                when b1 =>
                    current_state <= b0;
                when b0 =>
                    data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
                    current_state <= b23;
            end case;
            end if;                 
        end if;
    end process;



--    ******************************* sende prozess funktioniert bis 16MHz sehr gut ********************************
--    spi_miso <= data_out_buffer(current_bit_miso);
--    
--    miso_counter : process(spi_sck, data_out_buffer, current_bit_miso, data_tx, spi_ss, data_request_internal)
--    begin
--        if rising_edge(spi_sck) then
--        
--            --request shit
--            if(current_bit_miso = 1) then
--                data_request_internal <= '1';
--            else
--                data_request_internal <= '0';
--            end if;
--            
--            if(data_request_internal = '1') then
--                data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
--            end if;
--        
--            if (current_bit_miso > 0) and spi_ss = '0' then
--                current_bit_miso <= current_bit_miso - 1;
--                --data_request <= '0';            
--            else
--                current_bit_miso <= N-1;
--                --data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
--                --data_request <= '1';                
--            end if;            
--        end if;
--    end process;
    
    
end Behavioral;
