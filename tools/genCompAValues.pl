#!/usr/bin/perl -w

print "PROGMEM uint16_t compAValues[] = {\n";

for (my $i = 1; $i <= 256; $i++)
{
    printf("0x%0004x ", 16000000/(8000*($i)) );

    print "," if ($i < 256);

    if ( ($i) % 10 == 0 )
    {
        print "\n";
    }
}

print "};\n";

