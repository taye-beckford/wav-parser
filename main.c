#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUMRAWBYTES 8000

typedef struct
{
  char chunkID[4];
  uint32_t chunkSize;
} ChunkHeader;

typedef struct
{
  ChunkHeader header;
  char format[4];
} RIFFChunk;

typedef struct
{
  ChunkHeader header;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
} FMTChunk;

typedef struct
{
  ChunkHeader header;
  uint8_t *data;
} DataChunk;

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <wav_file>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "rb");
  if (!file)
  {
    perror("Failed to open file");
    return 1;
  }

  RIFFChunk riff;
  fread(&riff, sizeof(RIFFChunk), 1, file);
  if (strncmp(riff.header.chunkID, "RIFF", 4) != 0 || strncmp(riff.format, "WAVE", 4) != 0)
  {
    printf("Not a valid WAV file\n");
    fclose(file);
    return 1;
  }

  printf("RIFF Chunk\n");
  printf("----------\n");
  printf("ChunkID: %.4s\n", riff.header.chunkID);
  printf("ChunkSize: %u\n", riff.header.chunkSize);
  printf("Format: %.4s\n\n", riff.format);

  FMTChunk fmt;
  fread(&fmt, sizeof(FMTChunk), 1, file);
  if (strncmp(fmt.header.chunkID, "fmt ", 4) != 0)
  {
    printf("Expected 'fmt ' chunk, but got %.4s\n", fmt.header.chunkID);
    fclose(file);
    return 1;
  }

  printf("FMT Chunk\n");
  printf("---------\n");
  printf("ChunkID: %.4s\n", fmt.header.chunkID);
  printf("ChunkSize: %u\n", fmt.header.chunkSize);
  printf("AudioFormat: %u\n", fmt.audioFormat);
  printf("NumChannels: %u\n", fmt.numChannels);
  printf("SampleRate: %u\n", fmt.sampleRate);
  printf("ByteRate: %u\n", fmt.byteRate);
  printf("BlockAlign: %u\n", fmt.blockAlign);
  printf("BitsPerSample: %u\n\n", fmt.bitsPerSample);

  ChunkHeader header;
  DataChunk dataChunk;

  while (fread(&header, sizeof(ChunkHeader), 1, file) == 1)
  {
    if (strncmp(header.chunkID, "data", 4) == 0)
    {
      printf("DATA Chunk\n");
      printf("---------\n");
      printf("ChunkID: %.4s\n", header.chunkID);
      printf("ChunkSize: %u\n\n", header.chunkSize);
      dataChunk.header = header;
      dataChunk.data = malloc(header.chunkSize * sizeof(uint8_t));
      if (!dataChunk.data)
      {
        printf("Failed to allocate memory for data chunk.\n");
        fclose(file);
        return 1;
      }
      if (fread(dataChunk.data, header.chunkSize, 1, file) == 1)
      {
        printf("RAW DATA - first %d bytes\n", NUMRAWBYTES);
        printf("---------\n");
        for (size_t i = 0; i < NUMRAWBYTES; i++)
        {
          printf("%02X ", dataChunk.data[i]);
        }
        printf("\n");
      }
      else
      {
        printf("Could not read the datachunk");
      };
      free(dataChunk.data);
    }
    else
    {
      // Skip the data for any non-data chunks we encounter
      printf("Skipping Chunk");
      fseek(file, header.chunkSize, SEEK_CUR);
    }
  }

  fclose(file);

  return 0;
}