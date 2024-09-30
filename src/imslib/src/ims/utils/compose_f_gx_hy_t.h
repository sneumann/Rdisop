#ifndef IMS_COMPOSE_F_GX_HY_T_H
#define IMS_COMPOSE_F_GX_HY_T_H

namespace ims {

/**
 * @brief Binary compose function object adapter.
 *
 * Processes the result of two unary operations that use different elements as parameters.
 * Implemented to reduce the usage of user-defined functors.
 *
 * @tparam OP1 Type of the binary operation
 * @tparam OP2 Type of the first unary operation
 * @tparam OP3 Type of the second unary operation
 *
 * @note This implementation is based on concepts from "The C++ Standard Library"
 *       by Nicolai Josuttis, but updated for modern C++ standards.
 */

template <class OP1, class OP2, class OP3>
class compose_f_gx_hy_t
{
public:
        // Define parameters
        using first_argument_type = typename OP2::argument_type;
        using second_argument_type = typename OP3::argument_type;
        using result_type = typename OP1::result_type;
        
private:
        OP1 op1;    // process: op1(op2(x),op3(y))
        OP2 op2;
        OP3 op3;
        
public:
        // constructor
        compose_f_gx_hy_t (const OP1& o1, const OP2& o2, const OP3& o3)
                : op1(o1), op2(o2), op3(o3) {
        }
        
        // function call
        result_type
        operator()(const first_argument_type& x,
                 const second_argument_type& y) const {
                return op1(op2(x),op3(y));
        }
};

/** 
 * Convenience function for the @c compose_f_gx_hy_t adapter
 */
template <class OP1, class OP2, class OP3>
inline compose_f_gx_hy_t<OP1,OP2,OP3>
compose_f_gx_hy (const OP1& o1, const OP2& o2, const OP3& o3) {
    return compose_f_gx_hy_t<OP1,OP2,OP3>(o1,o2,o3);
}

}

#endif // IMS_COMPOSE_F_GX_HY_T_H
