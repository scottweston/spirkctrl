/*  Copyright (C) 2014 Scott Weston. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of
    conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list
    of conditions and the following disclaimer in the documentation and/or other materials
    provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY SCOTT WESTON "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
    OF SUCH DAMAGE.
*/

// IRPIN is the pin the IR Reciever is connected to
#define IRPIN A0
// LEDPIN is the pin the IR Tranmitter is connected to
#define LEDPIN A1
// ACTPIN is the pin an general LED is attached to (lights to indicate learning)
#define ACTPIN D7
// The maximum sequence length before overflow
#define MAXLEN 256

// Interrupt data when learning:

volatile unsigned int irData[MAXLEN];
volatile unsigned int position = 0;
volatile unsigned int overflow = 0;

// Sequences:

// Toshiba RAV SM800

const unsigned int airOff[]     = { 4470,4485,507,1663,507,579,506,1663,507,1664,506,580,505,582,502,1666,505,582,502,584,501,1668,503,584,500,586,499,1668,503,1666,504,584,501,1667,503,584,501,1666,506,1665,504,1666,504,1666,504,586,499,1667,503,1667,504,1666,504,583,502,584,501,584,500,586,500,1666,504,584,501,585,500,1669,501,1668,502,1668,503,584,501,586,498,588,497,587,498,588,497,588,497,588,498,586,498,1670,501,1668,502,1668,503,1667,503,1669,502,5246,4409,4490,503,1667,503,586,499,1668,502,1669,502,585,499,588,498,1668,502,585,500,586,499,1667,504,584,500,586,499,1668,503,1668,502,585,500,1668,502,586,499,1668,502,1668,503,1667,503,1667,503,583,502,1667,504,1665,505,1666,504,583,502,584,501,586,499,587,498,1669,501,588,497,587,498,1669,501,1668,503,1667,503,584,501,586,499,585,500,586,499,588,497,587,498,587,497,589,497,1670,500,1673,497,1669,502,1668,502,1670,501,0 };
const unsigned int airAutoOn[]  = { 4485,4482,511,1659,510,575,510,1661,509,1661,509,576,508,577,508,1662,508,577,508,577,508,1663,508,577,509,576,507,1663,508,1662,508,577,508,1662,508,577,508,577,508,578,507,1663,507,1663,508,1662,508,1663,507,1663,508,1662,508,1662,508,1662,508,577,508,577,508,577,508,577,508,577,508,578,507,1663,507,1663,507,1663,508,1662,508,577,508,577,508,578,507,1663,507,578,507,578,507,578,507,578,507,1663,507,1664,507,1663,508,5243,4411,4487,507,1663,507,578,507,1663,507,1663,508,577,507,578,507,1664,508,576,508,578,507,1663,507,578,507,578,507,1663,507,1663,508,577,508,1663,508,577,508,577,507,578,507,1663,507,1664,507,1663,507,1663,507,1663,508,1662,508,1662,508,1662,508,577,508,577,508,578,507,578,507,578,507,578,507,1663,507,1663,508,1663,507,1663,507,578,507,578,507,578,507,1663,508,577,507,578,507,578,507,578,507,1664,507,1663,507,1663,509,0 };
const unsigned int airHeatOn[]  = { 4482,4483,510,1660,510,575,509,1661,509,1661,510,575,509,576,509,1662,508,577,508,577,508,1662,508,577,508,577,508,1663,507,1663,508,577,507,1663,508,1662,508,577,508,1663,507,1663,508,1662,508,1662,508,1662,508,1662,509,576,509,1662,508,577,507,578,507,578,507,578,507,578,507,578,507,578,507,1664,507,1663,507,1663,507,1663,508,1662,508,577,508,577,508,1663,507,577,508,578,507,578,507,578,507,578,507,1663,507,1663,508,5244,4411,4486,507,1663,508,577,507,1664,507,1663,507,578,507,578,507,1663,507,578,507,578,507,1663,508,577,508,577,507,1664,507,1663,507,578,507,1663,508,1662,508,577,508,1663,507,1663,508,1662,508,1662,508,1662,508,1663,508,579,505,1663,508,577,508,577,508,577,507,579,506,578,507,578,507,578,507,1664,507,1663,507,1663,507,1663,508,1662,508,577,508,577,508,1663,507,578,507,578,507,578,507,578,507,578,507,1663,507,1664,507,0 };
const unsigned int airCoolOn[]  = { 4489,4482,511,1660,509,575,510,1661,509,1661,509,576,509,576,508,1662,509,576,509,576,508,1663,508,577,508,577,508,1662,508,1662,508,577,508,1663,507,1663,511,574,508,1662,508,1662,508,1662,508,1663,508,1662,508,1662,508,577,508,1662,508,577,508,577,508,577,508,577,508,577,508,578,507,578,507,1663,507,1663,508,1662,508,577,508,577,508,577,508,578,507,1663,507,578,507,578,507,578,507,1663,507,1664,507,1663,508,1662,507,5244,4412,4486,507,1663,507,578,507,1663,507,1663,508,577,508,577,508,1663,507,577,508,578,507,1663,507,578,507,578,507,1663,507,1664,507,577,508,1663,507,1663,508,577,508,1662,508,1662,508,1663,507,1663,508,1662,508,1662,508,577,508,1662,508,577,508,577,508,577,508,577,508,578,507,578,507,578,507,1663,507,1664,507,1663,507,577,508,578,507,578,507,578,507,1663,507,578,507,578,507,578,507,1664,507,1663,507,1663,507,1663,508,0 };

// ----------------------------------------------------------------------------------------

// example invokation:
// $ curl https://api.spark.io/v1/devices/SPARK_ID/replay -d access_token=API_TOKEN -d "args=heaton"
// $ curl https://api.spark.io/v1/devices/SPARK_ID/learn -d access_token=API_TOKEN

void setup() {
    pinMode(LEDPIN, OUTPUT);
    pinMode(ACTPIN, OUTPUT);
    
    Spark.function("replay", doReplay);
    Spark.function("learn", doLearn);
    
    Serial.begin(115200);
    
    _doUpdate("Waiting...");
}

void loop() {
    // all actions are invoked via spark function calls
}

// Internal functions ---------------------------------------------------------------------

// Called via Interrupt handler when a transition is detected on IRPIN

void _irRecv() {
  if (position > MAXLEN) {
      overflow++;
      return;
  }
  irData[position++] = micros();
}

// transmit a 38khz carrier signal on sig=HIGH, nothing but delay on sig=LOW

void _irTransmit(bool sig, unsigned int microseconds) {
    
    if (sig) {
        for (unsigned int i = 0; i < (microseconds / 26); i++) {
            digitalWrite(LEDPIN, HIGH);
            delayMicroseconds(10);
            digitalWrite(LEDPIN, LOW);
            delayMicroseconds(10);
        }
    } else {
        digitalWrite(LEDPIN, LOW);
        delayMicroseconds(microseconds-2);
    }
}

// it's highly likely this will fail if the core goes off and does some task
// up the chain, we should probably turn off wifi during the sequence replay
// to stop anything higher level taking the processor off us mid-sequence.
// But for now I'm just playing and the occasional failure is OK with me.
// If I stay away from toggle sequences then I can simply just replay the
// sequence a few times to ensure it takes.
//
// I could even use the time delta between the actual and calculated
// transmit times as a signal on if I need to retransmit due to possible
// interference.

int _playSequence(const unsigned int seq[]) {
    _doUpdate("Play Sequence");
    
    // ensure the IR is off and be quiet for a little bit
    
    digitalWrite(LEDPIN, LOW);
    delay(150);
    
    // calculate estimated sequence time

    unsigned int length = 0;
    
    for (int offset = 0; offset < MAXLEN; offset++) {
        if (seq[offset] == 0) break;
        length += seq[offset];
    }
    
    // now replay the sequence
    
    int start = micros();

    bool flip = true;
    for (int offset = 0; offset < MAXLEN; offset++) {
        if (seq[offset] == 0) break;
        _irTransmit(flip, seq[offset]);
        flip = !flip;
    }
    digitalWrite(LEDPIN, LOW);

    int delta = micros() - start - length;
    int error = length * 0.1;
    
    // if (delta > (length * 0.10))
    //   { 'something bad happened and the sequence probably failed and we should try it again.' }
    
    if (abs(delta) > error) {
        _doUpdate("FAILED: delta(" + String(delta) + "ms) > error(" + String(error) + ")");
        return(-1);
    } else {
        _doUpdate("OKAY: delta(" + String(delta) + "ms) < error(" + String(error) + ")");
        return(0);
    }
}

// while testing I had this function display via an OLED display, but switched to Serial to remove
// dependency

int _doUpdate(String args) {
    Serial.println(args);
    return(0);
}

// Exported functions ---------------------------------------------------------------------

// Output a sequence on the serial port which can be copied into a const array above

int doLearn(String args) {
    
    _doUpdate("Send IR now:");
    
    // put device into learning mode for 10 seconds
    
    attachInterrupt(IRPIN, _irRecv, CHANGE);
    delay(10000);
    detachInterrupt(IRPIN);
    
    // less than 20 state changes seems too low and might be caused by sun/fluro
    
    if (position > 20) {
        digitalWrite(ACTPIN, HIGH);  // indicate learning
        
        _doUpdate("Recv: (" + String(position) + ", " + String(overflow) + ");");
        
        Serial.println();
        Serial.println("datapoints ... : " + String(position));
        Serial.println("overflow ..... : " + String(overflow));
        
        Serial.println("Raw data:");
        for (unsigned int i = 0; i < position; i++) {
            Serial.print(irData[i]);
            Serial.print("/");
        }
        Serial.println();
        
        Serial.println("Calculated Period data (copy this one):");
        for (unsigned int i = 1; i < position; i++) {
            int period = irData[i] - irData[i-1];
            
            Serial.print(period);
            
            if (i != position-1) {
                Serial.print(",");
            } else {
                Serial.println(",0"); // 0 is used to indicate end of sequence
            }
        }
        
        // reset everything for the next time
        position = 0;
        overflow = 0;
        
        digitalWrite(ACTPIN, LOW);
        
        return(0);
    } else {
        _doUpdate("Insufficient IR");
        
        // reset counters for next time
        position = 0;
        overflow = 0;
        
        return(-1);
    }
}

int doReplay(String args) {
    args.trim();
    args.toUpperCase();
    
    if (args.equals("AUTOON")) {
        _playSequence(airAutoOn);
    } else if (args.equals("AIROFF")) {
        _playSequence(airOff);
    } else if (args.equals("HEATON")) {
        _playSequence(airHeatOn);
    } else if (args.equals("COOLON")) {
        _playSequence(airCoolOn);
    } else {
        return(-1);
    }
    return(0);
}

