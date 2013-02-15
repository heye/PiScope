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
            clk : in STD_LOGIC;
            adc_data : in STD_LOGIC_VECTOR(9 downto 0);
            clk_adc : out STD_LOGIC;
            s1 : in STD_LOGIC;
           MOSI : in STD_LOGIC;
           MISO : out STD_LOGIC;
           SCK : in STD_LOGIC;
           SS : in STD_LOGIC;
           --clk : in STD_LOGIC;
           led : out STD_LOGIC_VECTOR(7 downto 0);
           --P3 : out STD_LOGIC (3 downto 0);
           wr     : out std_logic;
           a      : out std_logic_vector(1 downto 0);
           d      : out std_logic_vector(6 downto 0);
           reset : in STD_LOGIC
           );
           
end spi_slave_new_wrapper;

architecture Behavioral of spi_slave_new_wrapper is
signal data_ready : STD_LOGIC;
signal data_rx : STD_LOGIC_VECTOR(7 downto 0);
signal data_buffer : STD_LOGIC_VECTOR(7 downto 0);
signal data_tx : STD_LOGIC_VECTOR(7 downto 0) := "00000000"; --:= "11110100";
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
signal fifo_dout : STD_LOGIC_VECTOR(9 downto 0);

begin

    clk_adc <= clk; 

    fifefgo : entity fifo_generator_v9_2_0
    port map(
        RST => s1,
        WR_CLK => clk,
        DIN => adc_data,
        WR_EN => fifo_we,
        FULL => fifo_full,
         
        RD_CLK => SCK,
        DOUT => fifo_dout,
        RD_EN => fifo_re,
        EMPTY => fifo_empty
    );

    spi_slave_core : entity work.spi_slave_new(Behavioral)
    generic map(
        N => 8   
    )
    port map( 
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
                digit1 <= data_rx(7 downto 4);
                digit0 <= data_rx(3 downto 0);
                
                data_buffer(7 downto 0) <= data_rx(7 downto 0);
                
                  --buffer(7 downto 0);
            end if;
        end if;
    end process;  
    
    
    
    
    spi_data_request: process (SCK, data_request, data_buffer, fifo_re, fifo_dout)
    begin 
        if rising_edge(SCK) then
            if data_request = '1' then
                fifo_re <= '1';
                --data_tx(7 downto 0) <= data_buffer(7 downto 0);
                --led(7 downto 0) <= data_buffer(7 downto 0);
                
                --counter test
                --data_tx(7 downto 0) <= data_counter;
                --data_counter <= data_counter + 1;
            else
                fifo_re <= '0';
            end if;     
            
            if fifo_re = '1' then
                data_tx(7 downto 0) <= fifo_dout(9 downto 2);
                led(7 downto 0) <= fifo_dout(9 downto 2);
            end if;       
        end if;
    end process;  
    
    
    
    fifo_write_enable : process(clk, fifo_full, fifo_empty)
    begin
        if rising_edge(clk) then
            if fifo_full = '1' then
                --pi_append <= '0';
                fifo_we <= '0';
            elsif fifo_empty = '1'then
                --pi_append <= '1';
                fifo_we <= '1';
            end if;
        end if;    
    end process;  
      
end Behavioral;
