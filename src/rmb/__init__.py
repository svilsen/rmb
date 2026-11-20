import sys

from importlib.metadata import version
from . import _core

def rmb(r: int, km: list[int], kp: list[int], c: list[int], method: str = "dynamic", type: bool = True) -> int | tuple[int, float] :
    """Count lattice points of a Manhattan ball in a bounded rectangular grid.

    Counts the number of points in the grid G = {-km_1, ..., kp_1} x ... x {-km_d, ..., kp_d} satisfying sum_i |u_i - c_i| <= r, i.e. the size |B(c | r, G)|.

    Arguments:
        r: Radius of the Manhattan ball.
        km: Lower extents k^- = (km_1, ..., km_d).
        kp: Upper extents k^+ = (kp_1, ..., kp_d).
        c: A point of the grid.
        method: The counting method,
            - "bruteforce" (or "bf"): enumerate every grid point, exponential in the grid size.
            - "exact" (or "ext"): exact inclusion-exclusion formula, O(d * 3^d) time-complexity.
            - "dynamic" (or "dp"): exact inclusion-exclusion formula, O(d * r) time-complexity and O(r) memory-complexity, found using dynamic programming.
            - "approximate" (or "app"): leading-term estimate of the exact formula, O(d * 3^d) time, with a guaranteed absolute error bound.
        type: Should the terms be grouped? NB: when 'method' == "bruteforce", this term is ignored.

    Returns:
        If 'method' is "bruteforce", "exact", or "dynamic", an 'int' is returned:
            - "count": an exact count of the number of points.
        If 'method' is "approximate" a 'tuple[int, float]' is returned:
            - "count": an estimate of the number of points.
            - "error": the guaranteed bound on |count - true value| for "approximate".
    """

    ##
    #
    PRECISION = 63
    UPPER_BOUND_GENENRAL = 2**PRECISION - 1

    ##
    #
    d = len(km)
    if d < 1 :
        raise ValueError(
            f"'d = len(km)' = {d}, but d must be >= 1."
        )
    elif d > PRECISION : 
        raise ValueError(
            f"'d' = {d}, but 'd' must be <= 63 ('d' is stored as a uint64_t in the C++ code and computes '1 << d', which is undefined for 'd' >= {PRECISION + 1})."
        )

    #
    if (len(km) != d) or (len(kp) != d) or (len(c) != d) :
        raise ValueError(
            f"'len(km)' = {len(km)}, 'len(kp)' = {len(kp)}, 'len(c)' = {len(c)}, but all must equal 'd' = {d}."
        )

    ##
    #
    if r < 0 :
        raise ValueError(
            f"r = {r}, but r must be >= 0 (r is a radius)."
        )

    #
    if r > UPPER_BOUND_GENENRAL : 
        raise ValueError(
            f"'r' = {r}, but 'r' must be <= 2**{PRECISION} - 1 = {UPPER_BOUND_GENENRAL} ('r' is stored as an int64_t in the C++ code)."
        )

    ##
    #
    if any(k < 0 for k in km) :
        raise ValueError(
            f"'km' = {km}, but all entries must be >= 0 ('km' is stored as a uint64_t in the C++ code)."
        )

    if any(k < 0 for k in kp) :
        raise ValueError(
            f"'kp' = {kp}, but all entries must be >= 0 ('kp' is stored as a uint64_t in the C++ code)."
        )

    #
    if any(k > UPPER_BOUND_GENENRAL for k in km) :
        raise ValueError(
            f"'km' = {km}, but all entries must be <= 2**{PRECISION} - 1 = {UPPER_BOUND_GENENRAL} ('km' is stored as a uint64_t in the C++ code)."
        )

    if any(k > UPPER_BOUND_GENENRAL for k in kp) :
        raise ValueError(
            f"'kp' = {kp}, but all entries must be <= 2**{PRECISION} - 1 = {UPPER_BOUND_GENENRAL} ('kp' is stored as a uint64_t in the C++ code)."
        )

    #
    UPPER_BOUND_K = (UPPER_BOUND_GENENRAL - d) // (2 * d)
    if any(k > UPPER_BOUND_K for k in km) :
        raise ValueError(
            f"'km' = {km}, but all entries must be <= K(d) = {UPPER_BOUND_K} for 'd' = {d} (to avoid overflow in the C++ code)."
        )

    if any(k > UPPER_BOUND_K for k in kp) :
        raise ValueError(
            f"'kp' = {kp}, but all entries must be <= K(d) = {UPPER_BOUND_K} for 'd' = {d} (to avoid overflow in the C++ code)."
        )

    ##
    #
    if any(c[i] < -km[i] for i in range(d)) :
        raise ValueError(
            f"'c' = {c}, but every 'c[i]' must be >= -'km[i]' ('c' must lie inside the grid)."
        )

    if any(c[i] > kp[i] for i in range(d)) :
        raise ValueError(
            f"'c' = {c}, but every 'c[i]' must be >= -'kp[i]' ('c' must lie inside the grid)."
        )

    ##
    #
    size_grd = 1
    for i in range(d) :
        size_grd *= km[i] + kp[i] + 1
        if size_grd >= UPPER_BOUND_GENENRAL :
            break

    if size_grd >= UPPER_BOUND_GENENRAL :
        size_bll = _core.delannoy(d, r)
        size_bnd = min(size_bll, size_grd)
        if size_bnd >= UPPER_BOUND_GENENRAL :
            raise ValueError(
                f"min(D(d, r), |G|) = {size_bnd}, but the ball size must be < 2**{PRECISION} (to avoid overflow in the C++ code)."
            )

    ##
    #
    error = 0
    if method == "bf" or method == "bruteforce" :
        count = _core.bruteforce(d, r, km, kp, c)
    elif method == "ext" or method == "exact":
        if type :
            count = _core.exact_grp(d, r, km, kp, c)
        else :
            count = _core.exact(d, r, km, kp, c)
    elif method == "dp" or method == "dynamic":
        if type :
            count = _core.dp_grp(d, r, km, kp, c)
        else :
            count = _core.dp(d, r, km, kp, c)
    elif method == "app" or method == "approximate":
        if d > 30 :
            raise ValueError(
                f"'d' = {d}, but 'd' must be <= 30 ('d' is stored as an int in the C++ code and computes '1 << d', which is undefined for 'd' >= {PRECISION + 1})."
            )

        if type :
            count, error = _core.approximate_grp(d, r, km, kp, c)
        else :
            count, error = _core.approximate(d, r, km, kp, c)
    else :
        raise ValueError(
            f"'method' only takes the values: 'bruteforce' (or 'bf'), 'exact' (or 'ext'), and 'approximate' (or 'app'); got the value '{method}'"
        )

    ##
    #
    if error > sys.float_info.epsilon :
        return count, error
    else :
        return count

__version__ = version("rmb")
__all__ = ["rmb"]

