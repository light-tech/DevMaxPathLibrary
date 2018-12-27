/*
 * Path processing library API
 *
 * Author: Lightech (lightech@outlook.com)
 *
 * Public Domain.
 */

#ifndef _DM_PATHS_H_
#define _DM_PATHS_H_

/*
 * Normalize a file or directory path in a file system
 * where by "normalizing", we meant removing standard symlinks
 * such as parent folder `..`, current folder `.` as well as
 * redundant path separator when there are more than 3
 * consecutive slashes `//.../`.
 */
void Normalize(const char *path, char *output, int output_length);

/*
 * Returns the pointer to where the file name starts.
 * Note that this method can also be used to extract the directory
 * i.e. the part prior to the file name.
 */
const char *GetFileName(const char *path);

/*
 * Returns pointer to the start of the file extension
 */
const char *GetFileExtension(const char *path);

/*
 * Substitute file extension by another extension
 */
void ChangeFileExtension(const char *path, const char *new_extension, char *output, char *length);

#endif /* _DM_PATHS_H_ */