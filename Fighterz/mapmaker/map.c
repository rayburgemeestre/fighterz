#include "header.h"

void loadmap()
{
int ret, cont = 1;
char buffer[512] = "";

    if (ret = file_select_ex(
        "Select map file",
        buffer,
        "txt;TXT;",
        sizeof(buffer),
        0, 0
    ) == 0 && cont == 1 && ! key[KEY_F12] )
    {
        die("");
    }
    alert("Opening map file:", buffer, "", "OK", NULL, 0, 0);
    strcpy(mapfile, buffer);

    // ret = popup_dialog(the_dialog, -1);

    {
        FILE *fi;
        int cnt = 0, x_width = 0, y_width = 0;
        char line[512];

        if (!(fi = fopen(buffer, "r")))
           die("Error opening file");

        while (fgets(line, 512 -1, fi) != NULL)
        {
            char *ptr = line;
            if (*ptr == '1')
            {
                if (strlen(line) <= 2)
                    die("Invalid line input while reading mapfile");
                strcpy(mapdata[cnt], (ptr + 2));
                if (strlen(ptr+2) > y_width)
                    y_width = strlen(ptr+2);
                x_width++;
                cnt++;
            }
        }

        {
            char tmp_x[32], tmp_y[32];
            sprintf(tmp_x, "Y %d", x_width);
            sprintf(tmp_y, "X %d", y_width - 1);
            alert("done", tmp_x, tmp_y, "", NULL, 0, 0);
        }
    }
}
