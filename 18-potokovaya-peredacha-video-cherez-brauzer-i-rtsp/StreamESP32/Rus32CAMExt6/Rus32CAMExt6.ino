
// #### [Разделить uint-32 на байты](https://forum.arduino.cc/t/solved-split-uint-32-to-bytes/532751)


typedef union
{
  uint32_t v;

  struct 
  {
    unsigned v00:4;
    unsigned v01:4;
    unsigned v10:4;
    unsigned v11:4;
    unsigned v20:4;
    unsigned v21:4;
    unsigned v30:4;
    unsigned v31:4;
  }
  as_nibbles;

  uint8_t as_bytes[4];
}
uint32_split_t, *uint32_split_p;

static uint32_split_t product;

void setup( void ) 
{
  Serial.begin(115200);
  //product.v = 0x1A2B3C4D; //for test purposes
  product.v   = 0x01010101; //for test purposes
}

void loop( void )
{
  product.v++;

  // uint8_t va = product>>24;// get 8 msb 24-31 // (uint8_t)((product>>24) & 0xFF);// get 8 msb 24-31
  // uint8_t vc = product>>20&0x0F; //get 8lsb 20-27and "zero out" 24-27  

  uint8_t va = product.as_bytes[0];
  uint8_t vc = product.as_nibbles.v21;
  Serial.print("PORTA = "); Serial.println(va);
  Serial.print("PORTС = "); Serial.println(vc);
  Serial.print("product.as_nibbles.v31 = "); Serial.println(product.as_nibbles.v31);
  Serial.print("product.as_nibbles.v00 = "); Serial.println(product.as_nibbles.v00);

  delay(2000); 

}
