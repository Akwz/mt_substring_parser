# Input options
MAX_MASK_LENGTH = 100
MAX_SEQUENCE_LENGTH = 1 * pow(1024, 3) # 1GB

# Generator options
DATA_FRAME_SIZE = 256 # Data that reads by one parsing unit
DATA_FETCH_SIZE_PER_FILE_ACCESS = 512 # Fetch amount of bytes per parser read operation


def generate_word(length: int):
	pass

def generate_full_fill_mask_data(mask: str):
	pass

def generate_data_with_layering(mask: str):
	pass

def generate_data_with_mask_layering(mask: str):
	pass

def generate_sequence():
	pass


if __name__ == "__main__":
	generate_sequence();