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
        );                                          -- 32bit serial word length is default
        --CPOL : std_logic := '0';                                        -- SPI mode selection (mode 0 default)
        --CPHA : std_logic := '0';                                        -- CPOL = clock polarity, CPHA = clock phase.
        --PREFETCH : positive := 3); 
    Port ( spi_miso : out STD_LOGIC;
           spi_mosi : in STD_LOGIC;
           spi_sck : in STD_LOGIC;
           spi_ss : in STD_LOGIC;
           data_tx : in STD_LOGIC_VECTOR(N-1 downto 0);
           data_rx : out STD_LOGIC_VECTOR(N-1 downto 0);
           data_request : out STD_LOGIC;
           data_ready : out STD_LOGIC);
end spi_slave_new;

architecture Behavioral of spi_slave_new is
    signal clk : STD_LOGIC;
    signal current_bit_mosi : natural range 0 to 15;
    signal current_bit_miso : natural range 0 to 15;
    
    signal data_in_buffer : STD_LOGIC_VECTOR(N-1 downto 0);
    signal data_out_buffer : STD_LOGIC_VECTOR(N-1 downto 0);
    signal bitM : STD_LOGIC;
    signal data_request_internal : STD_LOGIC;
begin
    clk <= spi_sck;
    data_request <= data_request_internal;
    
    
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
    

        
--        miso_counter : process(clk, data_out_buffer, current_bit_miso, data_tx, spi_ss, data_request_internal)
--        begin
--            if rising_edge(clk) then
--            
--                --request shit
--                if(current_bit_miso = 1) then
--                    data_request_internal <= '1';
--                else
--                    data_request_internal <= '0';
--                end if;
--                
--                if(data_request_internal = '1') then
--                    data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
--                    --data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
--                end if;
--            
--                if (current_bit_miso > 0) and spi_ss = '0' then
--                    spi_miso <= data_out_buffer(current_bit_miso - 1);
--                    current_bit_miso <= current_bit_miso - 1;
--                    --data_request <= '0';
--                else
--                    current_bit_miso <= N-1;
--                    
--                    
--                    spi_miso <= data_tx(N-1);
--                    --data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
--                    --data_request <= '1';                
--                end if;            
--            end if;
--        end process;


--    ******************************* sende prozess funktioniert bis 24MHz sehr gut ********************************
    spi_miso <= data_out_buffer(current_bit_miso);
    
    miso_counter : process(clk, data_out_buffer, current_bit_miso, data_tx, spi_ss, data_request_internal)
    begin
        if rising_edge(clk) then
        
            --request shit
            if(current_bit_miso = 1) then
                data_request_internal <= '1';
            else
                data_request_internal <= '0';
            end if;
            
            if(data_request_internal = '1') then
                data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
            end if;
        
            if (current_bit_miso > 0) and spi_ss = '0' then
                current_bit_miso <= current_bit_miso - 1;
                --data_request <= '0';
            else
                current_bit_miso <= N-1;
                --data_out_buffer(N-1 downto 0) <= data_tx(N-1 downto 0);
                --data_request <= '1';                
            end if;            
        end if;
    end process;
    
    
end Behavioral;
