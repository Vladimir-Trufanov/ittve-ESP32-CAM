/**
 * BasicHTTPSClient.ino
 * by tve modified: 05.01.2025
**/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

/*
// This is a Google Trust Services cert, the root Certificate Authority that
// signed the server certificate for the demo server https://jigsaw.w3.org in this
// example. This certificate is valid until Jan 28 00:00:42 2028 GMT
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDejCCAmKgAwIBAgIQf+UwvzMTQ77dghYQST2KGzANBgkqhkiG9w0BAQsFADBX\n" \
"MQswCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UE\n" \
"CxMHUm9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTIzMTEx\n" \
"NTAzNDMyMVoXDTI4MDEyODAwMDA0MlowRzELMAkGA1UEBhMCVVMxIjAgBgNVBAoT\n" \
"GUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBMTEMxFDASBgNVBAMTC0dUUyBSb290IFI0\n" \
"MHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE83Rzp2iLYK5DuDXFgTB7S0md+8Fhzube\n" \
"Rr1r1WEYNa5A3XP3iZEwWus87oV8okB2O6nGuEfYKueSkWpz6bFyOZ8pn6KY019e\n" \
"WIZlD6GEZQbR3IvJx3PIjGov5cSr0R2Ko4H/MIH8MA4GA1UdDwEB/wQEAwIBhjAd\n" \
"BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDwYDVR0TAQH/BAUwAwEB/zAd\n" \
"BgNVHQ4EFgQUgEzW63T/STaj1dj8tT7FavCUHYwwHwYDVR0jBBgwFoAUYHtmGkUN\n" \
"l8qJUC99BM00qP/8/UswNgYIKwYBBQUHAQEEKjAoMCYGCCsGAQUFBzAChhpodHRw\n" \
"Oi8vaS5wa2kuZ29vZy9nc3IxLmNydDAtBgNVHR8EJjAkMCKgIKAehhxodHRwOi8v\n" \
"Yy5wa2kuZ29vZy9yL2dzcjEuY3JsMBMGA1UdIAQMMAowCAYGZ4EMAQIBMA0GCSqG\n" \
"SIb3DQEBCwUAA4IBAQAYQrsPBtYDh5bjP2OBDwmkoWhIDDkic574y04tfzHpn+cJ\n" \
"odI2D4SseesQ6bDrarZ7C30ddLibZatoKiws3UL9xnELz4ct92vID24FfVbiI1hY\n" \
"+SW6FoVHkNeWIP0GCbaM4C6uVdF5dTUsMVs/ZbzNnIdCp5Gxmx5ejvEau8otR/Cs\n" \
"kGN+hr/W5GvT1tMBjgWKZ1i4//emhA1JG1BbPzoLJQvyEotc03lXjTaCzv8mEbep\n" \
"8RqZ7a2CPsgRbuvTPBwcOMBBmuFeU88+FSBX6+7iP0il8b4Z0QFqIwwMHfs/L6K1\n" \
"vepuoxtGzi4CZ68zJpiq1UvSqTbFJjtbD4seiMHl\n" \
"-----END CERTIFICATE-----\n";
*/

/*
const char *rootCACertificate = R"string_literal(
-----BEGIN CERTIFICATE-----
MIIDejCCAmKgAwIBAgIQf+UwvzMTQ77dghYQST2KGzANBgkqhkiG9w0BAQsFADBX
MQswCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UE
CxMHUm9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTIzMTEx
NTAzNDMyMVoXDTI4MDEyODAwMDA0MlowRzELMAkGA1UEBhMCVVMxIjAgBgNVBAoT
GUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBMTEMxFDASBgNVBAMTC0dUUyBSb290IFI0
MHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE83Rzp2iLYK5DuDXFgTB7S0md+8Fhzube
Rr1r1WEYNa5A3XP3iZEwWus87oV8okB2O6nGuEfYKueSkWpz6bFyOZ8pn6KY019e
WIZlD6GEZQbR3IvJx3PIjGov5cSr0R2Ko4H/MIH8MA4GA1UdDwEB/wQEAwIBhjAd
BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDwYDVR0TAQH/BAUwAwEB/zAd
BgNVHQ4EFgQUgEzW63T/STaj1dj8tT7FavCUHYwwHwYDVR0jBBgwFoAUYHtmGkUN
l8qJUC99BM00qP/8/UswNgYIKwYBBQUHAQEEKjAoMCYGCCsGAQUFBzAChhpodHRw
Oi8vaS5wa2kuZ29vZy9nc3IxLmNydDAtBgNVHR8EJjAkMCKgIKAehhxodHRwOi8v
Yy5wa2kuZ29vZy9yL2dzcjEuY3JsMBMGA1UdIAQMMAowCAYGZ4EMAQIBMA0GCSqG
SIb3DQEBCwUAA4IBAQAYQrsPBtYDh5bjP2OBDwmkoWhIDDkic574y04tfzHpn+cJ
odI2D4SseesQ6bDrarZ7C30ddLibZatoKiws3UL9xnELz4ct92vID24FfVbiI1hY
+SW6FoVHkNeWIP0GCbaM4C6uVdF5dTUsMVs/ZbzNnIdCp5Gxmx5ejvEau8otR/Cs
kGN+hr/W5GvT1tMBjgWKZ1i4//emhA1JG1BbPzoLJQvyEotc03lXjTaCzv8mEbep
8RqZ7a2CPsgRbuvTPBwcOMBBmuFeU88+FSBX6+7iP0il8b4Z0QFqIwwMHfs/L6K1
vepuoxtGzi4CZ68zJpiq1UvSqTbFJjtbD4seiMHl
-----END CERTIFICATE-----
)string_literal";
*/

// Определяем сертификат AlphaSSL, установленный для сайта www.kwinflat.ru.
// Этот сертификат действителен до 03.07.2026.
const char *rootCACertificate = R"string_literal(
-----BEGIN CERTIFICATE-----
MIIGXjCCBUagAwIBAgIMSZt6q1vAkQnkP04tMA0GCSqGSIb3DQEBCwUAMFUxCzAJ
BgNVBAYTAkJFMRkwFwYDVQQKExBHbG9iYWxTaWduIG52LXNhMSswKQYDVQQDEyJH
bG9iYWxTaWduIEdDQyBSNiBBbHBoYVNTTCBDQSAyMDIzMB4XDTI1MDYwMTE3NDgw
OVoXDTI2MDcwMzE3NDgwOFowGjEYMBYGA1UEAxMPd3d3Lmt3aW5mbGF0LnJ1MIIB
IjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA+GmY63kiTPIxFJmQScxFQ9pf
ZN81MSGEfHPbLOOnUqgVgHb17XEIVZce9F415aQxIFge3arb34/3aLRePAiRaYR3
deonAT/Y4sMiZZBRJD0Iu7J2NNrke8vL/yIOZnUkxkeeEe5OMB7BxniCCw33VTOz
ANRlNDuCbHuqhaIFALItE475gHoOq0HigqFesLfsIukUFpnvr/Et4aKT3QTIs2Ne
bnfXK5JaYxmouSxB0Ns6VmTa57a5FCdOMHL9VkV17EgxsjgK4Swl8xupnfHCSXfA
uw+3T/fTK8SwNYavZBonXhe44nLXEQve4Av8BiMsJcCFp0epk/fD5cLyAIBcxwID
AQABo4IDZzCCA2MwDgYDVR0PAQH/BAQDAgWgMAwGA1UdEwEB/wQCMAAwgZkGCCsG
AQUFBwEBBIGMMIGJMEkGCCsGAQUFBzAChj1odHRwOi8vc2VjdXJlLmdsb2JhbHNp
Z24uY29tL2NhY2VydC9nc2djY3I2YWxwaGFzc2xjYTIwMjMuY3J0MDwGCCsGAQUF
BzABhjBodHRwOi8vb2NzcC5nbG9iYWxzaWduLmNvbS9nc2djY3I2YWxwaGFzc2xj
YTIwMjMwVwYDVR0gBFAwTjAIBgZngQwBAgEwQgYKKwYBBAGgMgoBAzA0MDIGCCsG
AQUFBwIBFiZodHRwczovL3d3dy5nbG9iYWxzaWduLmNvbS9yZXBvc2l0b3J5LzBE
BgNVHR8EPTA7MDmgN6A1hjNodHRwOi8vY3JsLmdsb2JhbHNpZ24uY29tL2dzZ2Nj
cjZhbHBoYXNzbGNhMjAyMy5jcmwwJwYDVR0RBCAwHoIPd3d3Lmt3aW5mbGF0LnJ1
ggtrd2luZmxhdC5ydTAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwHwYD
VR0jBBgwFoAUvQW384qTPHPLefoPhRKhd5YYkXQwHQYDVR0OBBYEFD7HR7nW2Pez
YEPcZCDICOIEmA3AMIIBfgYKKwYBBAHWeQIEAgSCAW4EggFqAWgAdwCUTkOH+uzB
74HzGSQmqBhlAcfTXzgCAT9yZ31VNy4Z2AAAAZcsm7vCAAAEAwBIMEYCIQD+iQ7A
tRZcovwnVcgi+qqkyyfKsNbhJqyu6I01ZiIFgAIhAIB1d5zKDBGzY6BDCh+8UfOl
YcQdlrkT1NWufN32ASYFAHUAyzj3FYl8hKFEX1vB3fvJbvKaWc1HCmkFhbDLFMMU
WOcAAAGXLJu6HQAABAMARjBEAiAGXSbAo7UC2qpNCZ9sFFtOpyI48DhMZwygyuae
nFJqdwIgKskVo+33JpKZ8WkMvcm64vGqXANpcNu2hzvt6AlTnaIAdgDCMX5XRRmj
Re5/ON6ykEHrx8IhWiK/f9W1rXaa2Q5SzQAAAZcsm7uSAAAEAwBHMEUCIFQ6nvox
l0HBE3UR4CereqvnLcjx+WXmrFhk3aBOFSwAAiEAhofyKw3G0/5e9FfAIKDm4hy/
7qciZWsFCqNLOSASGXEwDQYJKoZIhvcNAQELBQADggEBALnpwvSXUhNezP9rNTK0
lHKNd0aflNKxvkk81Z/PBXQOMtjKxDIYsqHTgraFY0TPwWrGlgDsFIXSIA8gknJo
WHV0XJRWCpCgwHxjE3Pw7XfdpKPaLFV3J3yhl/VU1hPOar+NcEMDHS5l3e9ZNNl+
ZA+6nnnKnryWezNfb4nbvmhpo189I/vy0BZw/zjTRZ+VGmcpzxyle+6Y/cNEfsU4
OsQa7kcC4SMujfMwjqDt23Fsd4aZY5zywvth8kc0Wvz7z3ylpR4e7iERX4P3ZkM1
sbITtyw4hr/anjS5/BDXxLm44kPxnJeGcj2ETbgkyS3U/H7HBmiqnpKvXVBg1UCLm+Y=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFjDCCA3SgAwIBAgIQfx8skC6D0OO2+zvuR4tegDANBgkqhkiG9w0BAQsFADBM
MSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSNjETMBEGA1UEChMKR2xv
YmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjAeFw0yMzA3MTkwMzQzMjVaFw0y
NjA3MTkwMDAwMDBaMFUxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9iYWxTaWdu
IG52LXNhMSswKQYDVQQDEyJHbG9iYWxTaWduIEdDQyBSNiBBbHBoYVNTTCBDQSAy
MDIzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA00Jvk5ADppO0rgDn
j1M14XIb032Aas409JJFAb8cUjipFOth7ySLdaWLe3s63oSs5x3eWwzTpX4BFkzZ
bxT1eoJSHfT2M0wZ5QOPcCIjsr+YB8TAvV2yJSyq+emRrN/FtgCSTaWXSJ5jipW8
SJ/VAuXPMzuAP2yYpuPcjjQ5GyrssDXgu+FhtYxqyFP7BSvx9jQhh5QV5zhLycua
n8n+J0Uw09WRQK6JGQ5HzDZQinkNel+fZZNRG1gE9Qeh+tHBplrkalB1g85qJkPO
J7SoEvKsmDkajggk/sSq7NPyzFaa/VBGZiRRG+FkxCBniGD5618PQ4trcwHyMojS
FObOHQIDAQABo4IBXzCCAVswDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsG
AQUFBwMBBggrBgEFBQcDAjASBgNVHRMBAf8ECDAGAQH/AgEAMB0GA1UdDgQWBBS9
BbfzipM8c8t5+g+FEqF3lhiRdDAfBgNVHSMEGDAWgBSubAWjkxPioufi1xzWx/B/
yGdToDB7BggrBgEFBQcBAQRvMG0wLgYIKwYBBQUHMAGGImh0dHA6Ly9vY3NwMi5n
bG9iYWxzaWduLmNvbS9yb290cjYwOwYIKwYBBQUHMAKGL2h0dHA6Ly9zZWN1cmUu
Z2xvYmFsc2lnbi5jb20vY2FjZXJ0L3Jvb3QtcjYuY3J0MDYGA1UdHwQvMC0wK6Ap
oCeGJWh0dHA6Ly9jcmwuZ2xvYmFsc2lnbi5jb20vcm9vdC1yNi5jcmwwIQYDVR0g
BBowGDAIBgZngQwBAgEwDAYKKwYBBAGgMgoBAzANBgkqhkiG9w0BAQsFAAOCAgEA
fMkkMo5g4mn1ft4d4xR2kHzYpDukhC1XYPwfSZN3A9nEBadjdKZMH7iuS1vF8uSc
g26/30DRPen2fFRsr662ECyUCR4OfeiiGNdoQvcesM9Xpew3HLQP4qHg+s774hNL
vGRD4aKSKwFqLMrcqCw6tEAfX99tFWsD4jzbC6k8tjSLzEl0fTUlfkJaWpvLVkpg
9et8tD8d51bymCg5J6J6wcXpmsSGnksBobac1+nXmgB7jQC9edU8Z41FFo87BV3k
CtrWWsdkQavObMsXUPl/AO8y/jOuAWz0wyvPnKom+o6W4vKDY6/6XPypNdebOJ6m
jyaILp0quoQvhjx87BzENh5s57AIOyIGpS0sDEChVDPzLEfRsH2FJ8/W5woF0nvs
BTqfYSCqblQbHeDDtCj7Mlf8JfqaMuqcbE4rMSyfeHyCdZQwnc/r9ujnth691AJh
xyYeCM04metJIe7cB6d4dFm+Pd5ervY4x32r0uQ1Q0spy1VjNqUJjussYuXNyMmF
HSuLQQ6PrePmH5lcSMQpYKzPoD/RiNVD/PK0O3vuO5vh3o7oKb1FfzoanDsFFTrw
0aLOdRW/tmLPWVNVlAb8ad+B80YJsL4HXYnQG8wYAFb8LhwSDyT9v+C1C1lcIHE7
nE0AAp9JSHxDYsma9pi4g0Phg3BgOm2euTRzw7R0SzU=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIFgzCCA2ugAwIBAgIORea7A4Mzw4VlSOb/RVEwDQYJKoZIhvcNAQEMBQAwTDEg
MB4GA1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjYxEzARBgNVBAoTCkdsb2Jh
bFNpZ24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMTQxMjEwMDAwMDAwWhcNMzQx
MjEwMDAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSNjET
MBEGA1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCAiIwDQYJ
KoZIhvcNAQEBBQADggIPADCCAgoCggIBAJUH6HPKZvnsFMp7PPcNCPG0RQssgrRI
xutbPK6DuEGSMxSkb3/pKszGsIhrxbaJ0cay/xTOURQh7ErdG1rG1ofuTToVBu1k
ZguSgMpE3nOUTvOniX9PeGMIyBJQbUJmL025eShNUhqKGoC3GYEOfsSKvGRMIRxD
aNc9PIrFsmbVkJq3MQbFvuJtMgamHvm566qjuL++gmNQ0PAYid/kD3n16qIfKtJw
LnvnvJO7bVPiSHyMEAc4/2ayd2F+4OqMPKq0pPbzlUoSB239jLKJz9CgYXfIWHSw
1CM69106yqLbnQneXUQtkPGBzVeS+n68UARjNN9rkxi+azayOeSsJDa38O+2HBNX
k7besvjihbdzorg1qkXy4J02oW9UivFyVm4uiMVRQkQVlO6jxTiWm05OWgtH8wY2
SXcwvHE35absIQh1/OZhFj931dmRl4QKbNQCTXTAFO39OfuD8l4UoQSwC+n+7o/h
bguyCLNhZglqsQY6ZZZZwPA1/cnaKI0aEYdwgQqomnUdnjqGBQCe24DWJfncBZ4n
WUx2OVvq+aWh2IMP0f/fMBH5hc8zSPXKbWQULHpYT9NLCEnFlWQaYw55PfWzjMpY
rZxCRXluDocZXFSxZba/jJvcE+kNb7gu3GduyYsRtYQUigAZcIN5kZeR1Bonvzce
MgfYFGM8KEyvAgMBAAGjYzBhMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTAD
AQH/MB0GA1UdDgQWBBSubAWjkxPioufi1xzWx/B/yGdToDAfBgNVHSMEGDAWgBSu
bAWjkxPioufi1xzWx/B/yGdToDANBgkqhkiG9w0BAQwFAAOCAgEAgyXt6NH9lVLN
nsAEoJFp5lzQhN7craJP6Ed41mWYqVuoPId8AorRbrcWc+ZfwFSY1XS+wc3iEZGt
Ixg93eFyRJa0lV7Ae46ZeBZDE1ZXs6KzO7V33EByrKPrmzU+sQghoefEQzd5Mr61
55wsTLxDKZmOMNOsIeDjHfrYBzN2VAAiKrlNIC5waNrlU/yDXNOd8v9EDERm8tLj
vUYAGm0CuiVdjaExUd1URhxN25mW7xocBFymFe944Hn+Xds+qkxV/ZoVqW/hpvvf
cDDpw+5CRu3CkwWJ+n1jez/QcYF8AOiYrg54NMMl+68KnyBr3TsTjxKM4kEaSHpz
oHdpx7Zcf4LIHv5YGygrqGytXm3ABdJ7t+uA/iU3/gKbaKxCXcPu9czc8FB10jZp
nOZ7BN9uBmm23goJSFmH63sUYHpkqmlD75HHTOwY3WzvUy2MmeFe8nI+z1TIvWfs
pA9MRf/TuTAjB0yPEL+GltmZWrSZVxykzLsViVO6LAUP5MSeGbEYNNVMnbrt9x+v
JJUEeKgDu+6B5dpffItKoZB0JaezPkvILFa9x8jvOOJckvB595yEunQtYQEgfn7R
8k8HWV+LLUNS60YMlOH1Zkd5d9VUWx+tJDfLRVpOoERIyNiwmcUVhAn21klJwGW4
5hpxbqCo8YLoRT5s1gLXCmeDBVrJpBA=
-----END CERTIFICATE-----
)string_literal";

// Not sure if WiFiClientSecure checks the validity date of the certificate. 
// Setting clock just to be sure...
void setClock() 
{
  configTime(0, 0, "pool.ntp.org");
  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) 
  {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }
  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}

WiFiMulti wifiMulti;

void setup() 
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  Serial.println();

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("OPPO A9 2020", "b277a4ee84e8");
  wifiMulti.addAP("TP-Link_B394", "18009217");

  // wait for WiFi connection
  Serial.print("Waiting for WiFi to connect...");
  while ((wifiMulti.run() != WL_CONNECTED)) 
  {
    Serial.print(".");
  }
  Serial.println(" connected");
  setClock();  
}

void loop() 
{
  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) 
  {
    client -> setCACert(rootCACertificate);
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
      Serial.print("[HTTPS] begin...\n");
      // Подключаемся к https://kwinflat.ru/Esp32/
      if (https.begin(*client, "https://kwinflat.ru/Esp32/")) 
      // if (https.begin(*client, "https://jigsaw.w3.org/HTTP/connection.html")) 
      { // HTTPS
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) 
        {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
          {
            String payload = https.getString();
            Serial.println(payload);
          }
        }
        else 
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      } 
      else 
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
      // End extra scoping block
    }
    delete client;
  } 
  else 
  {
    Serial.println("Unable to create client");
  }

  Serial.println();
  Serial.println("Waiting 10s before the next round...");
  delay(10000);
}
