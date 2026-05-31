import shutil
from pathlib import Path


class FileDetection:
    home_path = Path("/Users/rohinboora")
    destination_root = home_path / "organized_files"

    @classmethod
    def find_files(cls):
        return [item for item in cls.home_path.iterdir()
                if item.is_file() and not item.name.startswith('.')]

    @classmethod
    def extension_folder(cls, item: Path) -> Path:
        ext = item.suffix.lower()
        if not ext:
            ext_name = "no_extension"
        else:
            ext_name = ext[1:]
        return cls.destination_root / ext_name

    @classmethod
    def unique_destination(cls, folder: Path, item: Path) -> Path:
        folder.mkdir(parents=True, exist_ok=True)
        destination = folder / item.name
        if not destination.exists():
            return destination

        stem = item.stem
        suffix = item.suffix
        count = 1
        while True:
            new_name = f"{stem}_{count}{suffix}"
            candidate = folder / new_name
            if not candidate.exists():
                return candidate
            count += 1

    @classmethod
    def move_all_by_extension(cls):
        files = cls.find_files()
        if not files:
            print(f"No files found in {cls.home_path}")
            return

        moved = []
        for item in files:
            folder = cls.extension_folder(item)
            destination = cls.unique_destination(folder, item)
            shutil.move(str(item), str(destination))
            moved.append((item.name, destination))

        for original, dest in moved:
            print(f"Moved {original} -> {dest}")


if __name__ == "__main__":
    FileDetection.move_all_by_extension()
