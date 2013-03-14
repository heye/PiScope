----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 01/13/2013 02:39:12 PM
-- Design Name: 
-- Module Name: spi_test - Behavioral
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

--library XilinxCoreLib;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;


entity spi_slave_new_wrapper is
    Port (  
            reset : in STD_LOGIC;
            clk : in STD_LOGIC;
            adc_data : in STD_LOGIC_VECTOR(9 downto 0);
            clk_adc : out STD_LOGIC;
           s1 : in STD_LOGIC;
           s2 : in STD_LOGIC;
           s3 : in STD_LOGIC;
           MOSI : in STD_LOGIC;
           MISO : out STD_LOGIC;
           SCK : in STD_LOGIC;
           SS : in STD_LOGIC;
           --clk : in STD_LOGIC;
           led : out STD_LOGIC_VECTOR(7 downto 0);
           --P3 : out STD_LOGIC (3 downto 0);
           wr     : out std_logic;
           a      : out std_logic_vector(1 downto 0);
           d      : out std_logic_vector(6 downto 0)
           --reset : in STD_LOGIC
           );
           
end spi_slave_new_wrapper;

architecture Behavioral of spi_slave_new_wrapper is
signal data_ready : STD_LOGIC;
signal data_rx : STD_LOGIC_VECTOR(23 downto 0);
signal data_buffer : STD_LOGIC_VECTOR(23 downto 0);
signal data_tx : STD_LOGIC_VECTOR(23 downto 0) := "110000000000000000000011"; --:= "11110100";
signal data_counter : STD_LOGIC_VECTOR(7 downto 0) := "00000001";
signal digit0 : STD_LOGIC_VECTOR(3 downto 0);
signal digit1 : STD_LOGIC_VECTOR(3 downto 0);
signal digit2 : STD_LOGIC_VECTOR(3 downto 0);
signal digit3 : STD_LOGIC_VECTOR(3 downto 0);
signal data_request : STD_LOGIC;

signal fifo_we : STD_LOGIC;
signal fifo_full : STD_LOGIC;
signal fifo_re : STD_LOGIC;
signal fifo_empty : STD_LOGIC;
signal fifo_dout : STD_LOGIC_VECTOR(10 downto 0);
signal fifo_din : STD_LOGIC_VECTOR(10 downto 0);

signal adc_counter : STD_LOGIC_VECTOR(9 downto 0) := "0000000000";
signal adc_time_mul : STD_LOGIC_VECTOR(9 downto 0) := "0010000000";
signal fifo_w_hold : STD_LOGIC;
signal fifo_w_hold_prev : STD_LOGIC;

signal  rpi_append : STD_LOGIC;

begin

    clk_adc <= clk; 
    
    led(0) <= fifo_empty;
    led(1) <= fifo_full;
    led(2) <= fifo_re;
    led(3) <= fifo_we;
    --led(4) <= data_ready;
    led(7 downto 4) <= "0110";

    fifo_din(9 downto 0) <= adc_data(9 downto 0);
    fifo_din(10) <= rpi_append;

    bigFifo : entity fifo_generator_v9_2_0
    port map(
        RST => not s1,  --active low
        WR_CLK => clk,
        DIN => fifo_din,
        WR_EN => fifo_we,
        FULL => fifo_full,
         
        RD_CLK => SCK,
        DOUT => fifo_dout,
        RD_EN => fifo_re,
        EMPTY => fifo_empty
    );

    spi_slave_core : entity work.spi_slave_new(Behavioral)
    generic map(
        N => 24   
    )
    port map( 
        reset => not reset,
        spi_mosi => MOSI,
        spi_miso => MISO,
        spi_sck => SCK,
        spi_ss => SS,
        
        data_tx => data_tx,--"11110100",
        data_rx => data_rx,
        data_ready => data_ready,
        data_request => data_request
    );
    
    display0 : entity work.display(Behavioral)
    port map(
        reset => reset,
        clk => SCK,
        digit0 => digit0,
        digit1 => digit1,
        digit2 => digit2,
        digit3 => digit3,
        write => wr,
        addr => a,
        data => d
    );     
   
   
   
    spi_data_input : process (SCK, data_ready, data_rx)
    begin
        if falling_edge(SCK) then
            if data_ready = '1' then
                --led(7 downto 0) <= data_rx(7 downto 0);
                --digit1 <= data_rx(23 downto 20);
               -- digit0 <= data_rx(19 downto 16);
                
                
                
                data_buffer <= data_rx;
                
                  --buffer(7 downto 0);
            end if;
        end if;
    end process;  
    
    
    
    
    spi_data_request: process (SCK, data_request, fifo_re)
    begin 
        if rising_edge(SCK) then
            if data_request = '1' then
                fifo_re <= '1';
            end if;
            if fifo_re = '1' then     --data was requested an new data from fifo is available
                fifo_re <= '0';
                
                data_tx(23 downto 22) <= "00";
                data_tx(21 downto 12) <= fifo_dout(9 downto 0); --CHB
                data_tx(11 downto 10) <= "00";
                data_tx(9 downto 0) <= fifo_dout(9 downto 0); --CHA 
                
--                data_tx(11) <= fifo_dout(10); -- fifo empty? -> rpi stop reading
--                data_tx(0) <= fifo_dout(10);
                
--                if s2 = '0' and s3 = '1' then    
--                    data_tx(23 downto 0) <= "000000001111111100111100";
--                    digit2 <= "0001";
--                elsif s2 = '1' and s3 = '0' then
--                    data_tx(23 downto 0) <= "000000001111111100111100";
--                    digit2 <= "0010";
--                elsif s2 = '1' and s3 = '1' then
--                    digit2 <= "0011";
--                    data_tx(23 downto 0) <= "000000001111111100111100";
--               elsif s2 = '0' and s3 = '0' then
--                    digit2 <= "0100";
--                    data_tx(23 downto 0) <= "000000001111111100111100";
--                end if;
                
                --led(7 downto 2) <= fifo_dout(9 downto 2);
                
                digit2 <= fifo_dout(5 downto 2);
                digit3 <= fifo_dout(9 downto 6);
                
            end if;       
        end if;
    end process;  
    
    
    rpi_info : process(clk, fifo_w_hold, fifo_w_hold_prev, fifo_we)
    begin
        if rising_edge(clk) then
            if (fifo_w_hold_prev = '1' and fifo_w_hold = '0') then -- fifo war voll wurde leer gelesen -> dont append next data
                rpi_append <= '0';
            elsif fifo_we = '1' then -- append wurde in fifo geschrieben -> rpi wurde benachrichtigt
                rpi_append <= '1';
            end if;
            fifo_w_hold_prev <= fifo_w_hold;
        end if;
    end process;
    
    
    
        fifo_write_hold : process(clk, fifo_full, fifo_empty)
        begin
            if rising_edge(clk) then
                if fifo_full = '1' then
                    fifo_w_hold <= '1';
                elsif fifo_empty = '1'then
                    fifo_w_hold <= '0';
                end if;
            end if;    
        end process;  
    
    fifo_write_enable : process(clk, fifo_w_hold, adc_counter, adc_time_mul)
    begin
        if rising_edge(clk) then
            if fifo_w_hold = '0' and adc_counter >= 250 then
                fifo_we <= '1';
                adc_counter <= adc_time_mul;
            else
                fifo_we <= '0';
                adc_counter <= adc_counter + 1;
            end if;
            
        end if;    
    end process;  
      
end Behavioral;
