import sys
import os
import numpy as np

# -----------------------------
# Normalization helpers
# -----------------------------
def normalize_vector(x):
    norm = np.linalg.norm(x)
    return x if norm == 0 else x / norm

def normalize_matrix(A):
    norm = np.linalg.norm(A)
    return A if norm == 0 else A / norm

# -----------------------------
# Generate vector
# -----------------------------
def generate_vector(n):
    os.makedirs("test/input", exist_ok=True)

    X = np.random.randn(n, 1)
    X = normalize_vector(X)

    path = f"test/input/{n}.txt"
    np.savetxt(path, X, fmt="%.6f")

    print(f"Vector saved to {path}")
    return X

# -----------------------------
# Generate matrix
# -----------------------------
def generate_matrix(n):
    os.makedirs("test/matrices", exist_ok=True)

    A = np.random.randn(n, n)
    A = normalize_matrix(A)

    path = f"test/matrices/{n}.txt"
    np.savetxt(path, A, fmt="%.6f")

    print(f"Matrix saved to {path}")
    return A

# -----------------------------
# Main pipeline
# -----------------------------
def generate_all(n):
    # Generate inputs
    A = generate_matrix(n)
    X = generate_vector(n)

    # Compute Y = A * X
    Y = np.matmul(A, X)

    # Save output
    os.makedirs("test/output", exist_ok=True)
    out_path = f"test/output/{n}.txt"
    np.savetxt(out_path, Y, fmt="%.6f")

    print(f"Output (Y = AX) saved to {out_path}")

# -----------------------------
# CLI entry
# -----------------------------
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <n>")
        sys.exit(1)

    n = int(sys.argv[1])
    generate_all(n)