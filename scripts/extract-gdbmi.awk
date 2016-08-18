#!/bin/awk -f
BEGIN {
    command = ""
}
{
    if ($1 == "start" && substr($2, 0, 4) == "args")
    {
        if (substr($2, 5, 3) == "[0]")
            command = ""
        $1 = ""
        $2 = ""
        command = command $0
    }
    if ($1 == "start" && $2 == "selected")
    {
        print command
    }
    if ($2 == "getline")
    {
        $1=""
        $2=""
        print
    }
}
END {
}
