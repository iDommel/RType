/*
** EPITECH PROJECT, 2022
** RType
** File description:
** ProtocolTraductor
*/

#ifndef PROTOCOLTRADUCTOR_HPP_
#define PROTOCOLTRADUCTOR_HPP_

namespace ecs
{
    class ProtocolTraductor
    {
        public:
            ProtocolTraductor();
            ~ProtocolTraductor();
            void setId(int id);
            bool serializeEvent();
            bool deserialize();
        private:
    };
} // namespace ecs

#endif /* !PROTOCOLTRADUCTOR_HPP_ */
