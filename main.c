// Codice da implementare dopo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3 || (argv[2][0] != 'p' && argv[2][0] != 's'))
    {
        printf("Err: usage %s 'link' p (playlist), s (single track)\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argv[2][0] == 'p')
    {
        char *link[] = 
        {
            "yt-dlp", 
            "-f", "bestaudio", 
            "-x", 
            "--audio-format", "mp3", 
            "--audio-quality", "192K", 
            "-o", "%(playlist_index)s - %(title)s.%(ext)s",
            argv[1], 
            NULL
        };
        execvp("yt-dlp", link);
        perror("execvp");
        return EXIT_FAILURE;
    } 
    else
    {
        char *link[] = 
        {
            "yt-dlp", 
            "-f", "bestaudio", 
            "-x", 
            "--audio-format", "mp3", 
            "--audio-quality", "192K",
            "--no-playlist",
            "-o", "%(title)s.%(ext)s", 
            argv[1], 
            NULL
        };
        execvp("yt-dlp", link);
        perror("execvp");
        return EXIT_FAILURE;
    }

    return 0;
}
